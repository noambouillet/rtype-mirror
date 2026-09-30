/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Server game loop
*/

#include "GameServer.hpp"

#include <cstddef>
#include <limits>
#include <thread>

#include "network/Protocol.hpp"

namespace rtype {

GameServer::GameServer(std::uint16_t port) : _network(port), _sessions{}, _nextSessionId(1), _tick(0)
{
}

/// Runs fixed network ticks until "running" becomes false.
void GameServer::run(const volatile std::sig_atomic_t& running)
{
	auto nextTick = Clock::now();

	while (running != 0) {
		tick();
		nextTick += TICK_DURATION;
		const auto now = Clock::now();
		if (nextTick < now) {
			nextTick = now + TICK_DURATION;
		}
		std::this_thread::sleep_until(nextTick);
	}
}

/// Processes queued packets and returns the current inputs for a future game system.
GameServer::TickInput GameServer::tick()
{
	_tick++;
	const auto now = Clock::now();

	for (const auto& datagram : _network.receivePending()) {
		handleDatagram(datagram, now);
	}
	return collectInput(now);
}

/// Validates one client datagram and updates its session.
void GameServer::handleDatagram(const network::Datagram& datagram, Clock::time_point now)
{
	const auto& bytes = datagram.bytes;
	const network::ParsedHeader parsed = network::parseClientHeader(bytes);

	if (parsed.error) {
		_network.send(datagram.sender, network::makeError(*parsed.error, parsed));
		return;
	}
	if (!parsed.header) {
		return;
	}
	const network::Header& header = *parsed.header;
	if (header.type == network::PacketType::JoinRequest) {
		handleJoinDatagram(datagram, header, parsed, now);
	} else {
		handleSessionDatagram(datagram, header, parsed, now);
	}
}

/// Validates a join request before reading its nonce.
void GameServer::handleJoinDatagram(const network::Datagram& datagram, const network::Header& header,
	const network::ParsedHeader& parsed, Clock::time_point now)
{
	if (const auto error = network::validateClientPayload(header, datagram.bytes)) {
		_network.send(datagram.sender, network::makeError(*error, parsed));
		return;
	}
	handleJoin(datagram, network::readU64(datagram.bytes, network::HEADER_SIZE), now);
}

/// Validates and applies a packet for an existing session.
void GameServer::handleSessionDatagram(const network::Datagram& datagram, const network::Header& header,
	const network::ParsedHeader& parsed, Clock::time_point now)
{
	for (std::size_t index = 0; index < _sessions.size(); index++) {
		auto& session = _sessions[index];
		if (!session || session->id() != header.sessionId) {
			continue;
		}
		if (const auto error = network::validateClientPayload(header, datagram.bytes)) {
			_network.send(datagram.sender, network::makeError(*error, parsed));
			return;
		}
		session->refresh(datagram.sender, now);
		if (header.type == network::PacketType::Disconnect) {
			removeSession(static_cast<std::uint8_t>(index), 1);
		} else {
			session->acceptInput({.sequence = header.sequence, .buttons = datagram.bytes[network::HEADER_SIZE]}, now);
		}
		return;
	}
	_network.send(
		datagram.sender,
		network::makeError(network::ErrorCode::InvalidSession, parsed));
}

/// Creates a session or repeats its acceptance for a known nonce.
void GameServer::handleJoin(const network::Datagram& datagram, std::uint64_t nonce, Clock::time_point now)
{
	if (handleRepeatedJoin(datagram, nonce, now)) {
		return;
	}

	const auto freeIndex = findFreeSlot();
	if (!freeIndex || _nextSessionId > std::numeric_limits<std::uint32_t>::max()) {
		sendJoinRejected(datagram, nonce);
		return;
	}
	const auto sessionId = static_cast<std::uint32_t>(_nextSessionId++);
	Session& session = _sessions[freeIndex.value()].emplace(datagram.sender, nonce, now, sessionId);
	sendJoinAccepted(session, static_cast<std::uint8_t>(freeIndex.value()));
}

/// Refreshes a session and repeats its acceptance when the nonce is known.
bool GameServer::handleRepeatedJoin(const network::Datagram& datagram, std::uint64_t nonce, Clock::time_point now)
{
	for (std::size_t index = 0; index < _sessions.size(); index++) {
		auto& session = _sessions[index];
		if (!session || session->nonce() != nonce) {
			continue;
		}
		session->refresh(datagram.sender, now);
		sendJoinAccepted(*session, static_cast<std::uint8_t>(index));
		return true;
	}
	return false;
}

/// Finds a player slot available for a new session.
std::optional<std::size_t> GameServer::findFreeSlot() const
{
	for (std::size_t index = 0; index < _sessions.size(); index++) {
		if (!_sessions[index]) {
			return index;
		}
	}
	return std::nullopt;
}

/// Sends an acceptance for a new or repeated join request.
void GameServer::sendJoinAccepted(const Session& session, std::uint8_t playerId)
{
	std::vector<std::uint8_t> payload;
	network::appendU64(payload, session.nonce());
	payload.push_back(playerId);
	_network.send(session.address(), network::makePacket(network::PacketType::JoinAccepted, 0, session.id(), payload));
}

/// Sends a server-full response for a join request.
void GameServer::sendJoinRejected(const network::Datagram& datagram, std::uint64_t nonce)
{
	std::vector<std::uint8_t> payload;
	network::appendU64(payload, nonce);
	payload.push_back(1);
	_network.send(datagram.sender, network::makePacket(network::PacketType::JoinRejected, 0, 0, payload));
}

/// Frees a player slot and announces the departure to the other sessions.
void GameServer::removeSession(std::uint8_t playerId, std::uint8_t reason)
{
	_sessions[playerId].reset();
	std::vector<std::uint8_t> payload;
	network::appendU64(payload, _tick);
	payload.push_back(playerId);
	payload.push_back(reason);
	broadcast(network::PacketType::PlayerLeft, payload);
}

/// Sends a session packet with the next nonzero outbound sequence.
void GameServer::sendTo(Session& session, network::PacketType type, const std::vector<std::uint8_t>& payload)
{
	const std::uint32_t sequence = session.takeNextSequence();
	_network.send(session.address(), network::makePacket(type, sequence, session.id(), payload));
}

/// Sends the same packet payload to every connected player.
void GameServer::broadcast(network::PacketType type, const std::vector<std::uint8_t>& payload)
{
	for (auto& session : _sessions) {
		if (session) {
			sendTo(*session, type, payload);
		}
	}
}

/// Expires silent sessions and returns fresh button states for this tick.
GameServer::TickInput GameServer::collectInput(Clock::time_point now)
{
	TickInput input{.buttons = {}, .connectedMask = 0};
	for (std::size_t index = 0; index < _sessions.size(); index++) {
		auto& session = _sessions[index];
		if (!session) {
			continue;
		}
		if (session->hasExpired(now, SESSION_TIMEOUT)) {
			removeSession(static_cast<std::uint8_t>(index), 2);
			continue;
		}
		input.connectedMask |= static_cast<std::uint8_t>(1 << index);
		input.buttons[index] = session->buttonsAt(now, INPUT_TIMEOUT);
	}
	return input;
}

}
