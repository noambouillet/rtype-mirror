/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Game server class definition
*/

#pragma once

#include <array>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_set>
#include <vector>

#include "ECS/Coordinator/Coordinator.hpp"
#include "Session.hpp"
#include "network/UdpServer.hpp"
#include "utils/network/Protocol.hpp"

namespace rtype {

class GameServer {
	static constexpr auto TICK_DURATION = std::chrono::microseconds(16667);
	static constexpr auto SNAPSHOT_INTERVAL = std::chrono::milliseconds(50);
	static constexpr auto INPUT_TIMEOUT = std::chrono::milliseconds(250);
	static constexpr auto SESSION_TIMEOUT = std::chrono::seconds(5);
	static constexpr std::size_t MAX_PLAYERS = 4;
	static constexpr float PLAYER_SPEED = 250.0f;
	static constexpr std::uint8_t BUTTON_UP = 0x01;
	static constexpr std::uint8_t BUTTON_DOWN = 0x02;
	static constexpr std::uint8_t BUTTON_LEFT = 0x04;
	static constexpr std::uint8_t BUTTON_RIGHT = 0x08;
	static_assert(MAX_PLAYERS <= 8, "connectedMask supports at most eight players");

	using Clock = std::chrono::steady_clock;

public:
	struct TickInput {
		std::array<std::uint8_t, MAX_PLAYERS> buttons;
		std::uint8_t connectedMask;
	};

	explicit GameServer(std::uint16_t port);
	void run(const volatile std::sig_atomic_t& running);
	TickInput tick();

private:
	struct TmpPosition {
		float x;
		float y;
	};

	struct TmpVelocity {
		float x;
		float y;
	};

	void handleDatagram(const network::Datagram& datagram, Clock::time_point now);
	void handleJoinDatagram(const network::Datagram& datagram, const network::Header& header, const network::ParsedHeader& parsed, Clock::time_point now);
	void handleSessionDatagram(const network::Datagram& datagram, const network::Header& header, const network::ParsedHeader& parsed, Clock::time_point now);
	void handleJoin(const network::Datagram& datagram, std::uint64_t nonce, Clock::time_point now);
	bool handleRepeatedJoin(const network::Datagram& datagram, std::uint64_t nonce, Clock::time_point now);
	std::optional<std::size_t> findFreeSlot() const;
	std::optional<std::uint32_t> createSessionId();
	void sendJoinAccepted(const Session& session, std::uint8_t playerId);
	void sendJoinRejected(const network::Datagram& datagram, std::uint64_t nonce);
	void removeSession(std::uint8_t playerId, std::uint8_t reason);
	void sendTo(Session& session, network::PacketType type, const std::vector<std::uint8_t>& payload);
	void broadcast(network::PacketType type, const std::vector<std::uint8_t>& payload);
	TickInput collectInput(Clock::time_point now);
	void syncPlayerEntities(const TickInput& input);
	void applyPlayerPhysics(const TickInput& input, float deltaTime);
	void updateWorld(const TickInput& input, float deltaTime);
	std::vector<std::uint8_t> buildSnapshot(const TickInput& input);

	network::UdpServer _network;
	ecs::Coordinator _coordinator;
	std::array<std::optional<Session>, MAX_PLAYERS> _sessions;
	std::array<std::optional<ecs::Entity>, MAX_PLAYERS> _playerEntities;
	std::unordered_set<std::uint32_t> _issuedSessionIds;
	std::uint64_t _tick;
};

}
