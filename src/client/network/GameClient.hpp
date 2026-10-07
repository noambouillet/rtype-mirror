/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Client protocol session
*/

#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "UdpClient.hpp"

namespace rtype::network {

enum class EntityType : std::uint8_t {
	Player = 1,
	Monster = 2,
	Laser = 3,
	Bonus = 4
};

struct WorldEntity {
	std::uint32_t entityId;
	EntityType entityType;
	std::uint8_t ownerPlayerId;
	std::uint8_t spriteId;
	std::uint8_t health;
	float x;
	float y;
};

struct WorldSnapshot {
	std::uint64_t tick;
	std::uint8_t connectedMask;
	std::uint8_t aliveMask;
	std::vector<WorldEntity> entities;
};

enum class ClientStatus : std::uint8_t { Joining,
	Connected,
	Rejected,
	TimedOut,
	Disconnected };

struct PlayerDeparture {
	std::uint8_t playerId;
	std::uint8_t reason;
};

class GameClient {
private:
	static constexpr auto JOIN_RETRY = std::chrono::milliseconds(500);
	static constexpr auto INPUT_INTERVAL = std::chrono::milliseconds(50);
	static constexpr unsigned int MAX_JOIN_ATTEMPTS = 10;
	static constexpr std::size_t SNAPSHOT_HEADER_SIZE = 11;
	static constexpr std::size_t ENTITY_RECORD_SIZE = 16;
	static constexpr std::size_t MAX_SNAPSHOT_ENTITIES = 64;
	using Clock = std::chrono::steady_clock;

	void handlePacket(const std::vector<std::uint8_t>& bytes);
	void handleJoinPacket(const Header& header, const std::vector<std::uint8_t>& bytes);
	void handleSessionPacket(const Header& header, const std::vector<std::uint8_t>& bytes);
	void handleSnapshot(const std::vector<std::uint8_t>& bytes);
	static std::optional<std::vector<WorldEntity>> parseEntities(const std::vector<std::uint8_t>& bytes, std::uint8_t entityCount);
	static float decodePosition(const std::vector<std::uint8_t>& bytes, std::size_t offset);
	void sendJoin();
	void sendInput(std::uint8_t buttons);
	std::uint32_t takeSequence();

	UdpClient _udp;
	std::uint64_t _nonce;
	std::uint32_t _sessionId;
	std::uint32_t _nextSequence;
	std::uint8_t _playerId;
	std::uint8_t _lastButtons;
	unsigned int _joinAttempts;
	Clock::time_point _lastJoin;
	Clock::time_point _lastInput;
	ClientStatus _status;
	std::optional<PlayerDeparture> _departure;
	std::optional<ErrorCode> _error;
	std::optional<WorldSnapshot> _worldSnapshot;

public:
	GameClient(const std::string& host, std::uint16_t port);
	void tick(std::uint8_t buttons);
	void disconnect();
	std::optional<PlayerDeparture> takeDeparture();
	std::optional<ErrorCode> takeError();
	[[nodiscard]] ClientStatus status() const;
	[[nodiscard]] std::uint8_t playerId() const;
	[[nodiscard]] std::uint32_t sessionId() const;
	[[nodiscard]] const std::optional<WorldSnapshot>& worldSnapshot() const;
};

}
