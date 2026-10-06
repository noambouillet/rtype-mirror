/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Client protocol session implementation
*/

#include "GameClient.hpp"

#include <random>
#include <utility>

#include "utils/RTypeError.hpp"
#include "utils/network/Protocol.hpp"

namespace rtype::network {

/// Generates a nonce across the full 64-bit protocol field.
static std::uint64_t generateNonce()
{
	std::random_device random;
	std::uniform_int_distribution<std::uint64_t> distribution;
	return distribution(random);
}

/// Creates a client with a fresh nonce for this join attempt.
GameClient::GameClient(const std::string& host, std::uint16_t port)
	: _udp(host, port), _nonce(generateNonce()), _sessionId(0), _nextSequence(1), _playerId(0), _lastButtons(0), _joinAttempts(0), _status(ClientStatus::Joining), _worldSnapshot{}
{
}

/// Processes server packets and refreshes input at the RFC's 50 ms interval.
void GameClient::tick(std::uint8_t buttons)
{
	if (buttons > MAX_BUTTON_STATE) {
		throw NetworkError("Reserved input buttons are set");
	}
	for (const auto& bytes : _udp.receivePending()) {
		handlePacket(bytes);
	}
	const auto now = Clock::now();
	if (_status == ClientStatus::Joining && (_joinAttempts == 0 || now - _lastJoin >= JOIN_RETRY)) {
		if (_joinAttempts == MAX_JOIN_ATTEMPTS) {
			_status = ClientStatus::TimedOut;
		} else {
			sendJoin();
		}
	}
	if (_status == ClientStatus::Connected && (_lastInput == Clock::time_point{} || buttons != _lastButtons || now - _lastInput >= INPUT_INTERVAL)) {
		sendInput(buttons);
	}
}

/// Sends an orderly departure once when an active session is closed.
void GameClient::disconnect()
{
	if (_status == ClientStatus::Connected) {
		_udp.send(makePacket(PacketType::Disconnect, takeSequence(), _sessionId, {}));
	}
	_status = ClientStatus::Disconnected;
}

/// Returns the latest departure notice, if one was received since the last call.
std::optional<PlayerDeparture> GameClient::takeDeparture()
{
	const auto departure = _departure;
	_departure.reset();
	return departure;
}

/// Returns the latest protocol error, if one was received since the last call.
std::optional<ErrorCode> GameClient::takeError()
{
	const auto error = _error;
	_error.reset();
	return error;
}

ClientStatus GameClient::status() const
{
	return _status;
}

std::uint8_t GameClient::playerId() const
{
	return _playerId;
}

std::uint32_t GameClient::sessionId() const
{
	return _sessionId;
}

/// Returns the latest accepted world snapshot for rendering.
const std::optional<WorldSnapshot>& GameClient::worldSnapshot() const
{
	return _worldSnapshot;
}

/// Accepts only valid replies for this join nonce or active session.
void GameClient::handlePacket(const std::vector<std::uint8_t>& bytes)
{
	const auto header = parseServerHeader(bytes);
	if (!header) {
		return;
	}
	if (_status == ClientStatus::Joining) {
		handleJoinPacket(*header, bytes);
	} else if (_status == ClientStatus::Connected) {
		handleSessionPacket(*header, bytes);
	}
}

/// Checks the nonce before accepting a join response.
void GameClient::handleJoinPacket(const Header& header, const std::vector<std::uint8_t>& bytes)
{
	if ((header.type != PacketType::JoinAccepted && header.type != PacketType::JoinRejected) || readU64(bytes, HEADER_SIZE) != _nonce) {
		return;
	}
	if (header.type == PacketType::JoinRejected) {
		_status = ClientStatus::Rejected;
	} else {
		_sessionId = header.sessionId;
		_playerId = bytes[HEADER_SIZE + 8];
		_status = ClientStatus::Connected;
	}
}

/// Records validated server notices for the active session.
void GameClient::handleSessionPacket(const Header& header, const std::vector<std::uint8_t>& bytes)
{
	if (header.type == PacketType::PlayerLeft && header.sessionId == _sessionId) {
		PlayerDeparture departure;
		departure.playerId = bytes[HEADER_SIZE + 8];
		departure.reason = bytes[HEADER_SIZE + 9];
		_departure = departure;
	} else if (header.type == PacketType::Error) {
		_error = static_cast<ErrorCode>(bytes[HEADER_SIZE]);
		if (*_error == ErrorCode::InvalidSession) {
			_status = ClientStatus::Disconnected;
		}
	} else if (header.type == PacketType::WorldSnapshot) {
		handleSnapshot(header, bytes);
	}
}

/// Decodes a signed 1/16-pixel network coordinate into logical pixels.
float GameClient::decodePosition(const std::vector<std::uint8_t>& bytes, std::size_t offset)
{
	const std::int32_t fixedPosition = static_cast<std::int32_t>(readU32(bytes, offset));
	return static_cast<float>(fixedPosition) / 16.0f;
}

/// Parses the entity records after verifying the exact snapshot packet size.
std::optional<std::vector<WorldEntity>> GameClient::parseEntities(const std::vector<std::uint8_t>& bytes,
	std::uint8_t entityCount)
{
	if (entityCount > MAX_SNAPSHOT_ENTITIES ||
		bytes.size() != HEADER_SIZE + SNAPSHOT_HEADER_SIZE + entityCount * ENTITY_RECORD_SIZE) {
		return std::nullopt;
	}

	std::vector<WorldEntity> entities;
	entities.reserve(entityCount);
	for (std::size_t index = 0; index < entityCount; index++) {
		const std::size_t recordOffset = HEADER_SIZE + SNAPSHOT_HEADER_SIZE + index * ENTITY_RECORD_SIZE;
		const std::uint8_t rawType = bytes[recordOffset + 4];
		const std::uint8_t ownerPlayerId = bytes[recordOffset + 5];
		const std::uint8_t spriteId = bytes[recordOffset + 6];
		const EntityType entityType = static_cast<EntityType>(rawType);

		if (entityType < EntityType::Player || entityType > EntityType::Bonus ||
			((entityType == EntityType::Player || entityType == EntityType::Laser) ? ownerPlayerId > 3 : ownerPlayerId != 255) ||
			(entityType == EntityType::Player && spriteId != ownerPlayerId)) {
			return std::nullopt;
		}
		entities.push_back({
			.entityId = readU32(bytes, recordOffset),
			.entityType = static_cast<EntityType>(entityType),
			.ownerPlayerId = ownerPlayerId,
			.spriteId = spriteId,
			.health = bytes[recordOffset + 7],
			.x = decodePosition(bytes, recordOffset + 8),
			.y = decodePosition(bytes, recordOffset + 12),
		});
	}
	return entities;
}

/// Replaces the cached world only with a complete, newer snapshot for this session.
void GameClient::handleSnapshot(const Header& header, const std::vector<std::uint8_t>& bytes)
{
	if (header.sessionId != _sessionId || bytes.size() < HEADER_SIZE + SNAPSHOT_HEADER_SIZE) {
		return;
	}
	const std::uint64_t tick = readU64(bytes, HEADER_SIZE);
	if (_worldSnapshot && tick <= _worldSnapshot->tick) {
		return;
	}
	const std::uint8_t entityCount = bytes[HEADER_SIZE + 10];
	auto entities = parseEntities(bytes, entityCount);
	if (!entities) {
		return;
	}
	_worldSnapshot = WorldSnapshot{
		.tick = tick,
		.connectedMask = bytes[HEADER_SIZE + 8],
		.aliveMask = bytes[HEADER_SIZE + 9],
		.entities = std::move(*entities),
	};
}

/// Retries the same request and nonce without creating duplicate sessions.
void GameClient::sendJoin()
{
	std::vector<std::uint8_t> payload;
	appendU64(payload, _nonce);
	_udp.send(makePacket(PacketType::JoinRequest, 0, 0, payload));
	_lastJoin = Clock::now();
	_joinAttempts++;
}

/// Sends the complete button state using the next nonzero sequence.
void GameClient::sendInput(std::uint8_t buttons)
{
	_udp.send(makePacket(PacketType::PlayerInput, takeSequence(), _sessionId, {buttons}));
	_lastButtons = buttons;
	_lastInput = Clock::now();
}

/// Advances the session sequence while skipping zero after wraparound.
std::uint32_t GameClient::takeSequence()
{
	const std::uint32_t sequence = _nextSequence++;
	if (_nextSequence == 0) {
		_nextSequence = 1;
	}
	return sequence;
}

}
