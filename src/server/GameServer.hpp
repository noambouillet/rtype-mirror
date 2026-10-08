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

#include "Session.hpp"
#include "game/GameWorld.hpp"
#include "network/UdpServer.hpp"
#include "utils/network/Protocol.hpp"

namespace rtype {

class GameServer {
	static constexpr auto TICK_DURATION = std::chrono::microseconds(16667);
	static constexpr auto SNAPSHOT_INTERVAL = std::chrono::milliseconds(50);
	static constexpr auto INPUT_TIMEOUT = std::chrono::milliseconds(250);
	static constexpr auto SESSION_TIMEOUT = std::chrono::seconds(5);
	static constexpr std::size_t SNAPSHOT_HEADER_SIZE = 11;
	static constexpr std::size_t ENTITY_RECORD_SIZE = 16;

	using Clock = std::chrono::steady_clock;

public:
	explicit GameServer(std::uint16_t port);
	void run(const volatile std::sig_atomic_t& running);
	GameWorld::Input tick();

private:
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
	GameWorld::Input collectInput(Clock::time_point now);
	std::vector<std::uint8_t> buildSnapshot(const GameWorld::Input& input);

	network::UdpServer _network;
	GameWorld _world;
	std::array<std::optional<Session>, GameWorld::MAX_PLAYERS> _sessions;
	std::unordered_set<std::uint32_t> _issuedSessionIds;
	std::uint64_t _tick;
};

}
