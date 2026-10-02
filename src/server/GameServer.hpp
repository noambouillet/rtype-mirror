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
#include <vector>

#include "Session.hpp"
#include "network/Protocol.hpp"
#include "network/UdpServer.hpp"

namespace rtype {

class GameServer {
	static constexpr auto TICK_DURATION = std::chrono::microseconds(16667);
	static constexpr auto INPUT_TIMEOUT = std::chrono::milliseconds(250);
	static constexpr auto SESSION_TIMEOUT = std::chrono::seconds(5);
	static constexpr std::size_t MAX_PLAYERS = 4;
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
	void handleDatagram(const network::Datagram& datagram, Clock::time_point now);
	void handleJoinDatagram(const network::Datagram& datagram, const network::Header& header, const network::ParsedHeader& parsed, Clock::time_point now);
	void handleSessionDatagram(const network::Datagram& datagram, const network::Header& header, const network::ParsedHeader& parsed, Clock::time_point now);
	void handleJoin(const network::Datagram& datagram, std::uint64_t nonce, Clock::time_point now);
	bool handleRepeatedJoin(const network::Datagram& datagram, std::uint64_t nonce, Clock::time_point now);
	std::optional<std::size_t> findFreeSlot() const;
	void sendJoinAccepted(const Session& session, std::uint8_t playerId);
	void sendJoinRejected(const network::Datagram& datagram, std::uint64_t nonce);
	void removeSession(std::uint8_t playerId, std::uint8_t reason);
	void sendTo(Session& session, network::PacketType type, const std::vector<std::uint8_t>& payload);
	void broadcast(network::PacketType type, const std::vector<std::uint8_t>& payload);
	TickInput collectInput(Clock::time_point now);

	network::UdpServer _network;
	std::array<std::optional<Session>, MAX_PLAYERS> _sessions;
	std::uint64_t _nextSessionId;
	std::uint64_t _tick;
};

}
