/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Entrypoint of the R-Type server
*/

#include <charconv>
#include <csignal>
#include <cstdint>
#include <exception>
#include <iostream>
#include <string_view>
#include <system_error>

#include "GameServer.hpp"
#include "utils/Logger.hpp"

namespace rtype {

static volatile std::sig_atomic_t running = 1;

static void stopServer([[maybe_unused]] int signal)
{
	running = 0;
}

/// Parses a decimal UDP port in the inclusive range 1 to 65535.
static bool parsePort(std::string_view text, std::uint16_t& port)
{
	unsigned int value = 0;

	const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
	if (error != std::errc{} || end != text.data() + text.size() || value == 0 || value > 65535) {
		return false;
	}
	port = static_cast<std::uint16_t>(value);
	return true;
}

}

int main(int argc, char** argv)
{
	std::uint16_t port = 6767;
	if (argc > 2 || (argc == 2 && !rtype::parsePort(argv[1], port))) {
		std::cerr << "Usage: r-type_server [udp-port]\n";
		return 2;
	}
	std::signal(SIGINT, rtype::stopServer);
	std::signal(SIGTERM, rtype::stopServer);
	rtype::Logger logger(std::cout);
	logger.setVerbose(true);
	try {
		rtype::GameServer server(port);
		logger.info("R-Type server listening on UDP port " + std::to_string(port));
		server.run(rtype::running);
	} catch (const std::exception& error) {
		logger.error(std::string("Server error: ") + error.what());
		return 1;
	}
	logger.info("Goodbye...");
	return 0;
}
