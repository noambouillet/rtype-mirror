/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Graphical client application loop
*/

#include "ClientApp.hpp"

#include <csignal>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include "client/SFML/Sfml.hpp"
#include "client/SFML/SpriteRegions.hpp"
#include "client/network/GameClient.hpp"
#include "utils/Logger.hpp"

namespace rtype {

static volatile std::sig_atomic_t running = 1;

static void stopClient([[maybe_unused]] int signal)
{
	running = 0;
}

/// Creates a separate logger for this client process.
static std::unique_ptr<Logger> makeLogger(const std::string& logFile)
{
	if (logFile.empty()) {
		return std::make_unique<Logger>(std::cout);
	}
	return std::make_unique<Logger>(std::filesystem::path(logFile));
}

/// Reports a change in the client's session state.
static bool reportStatus(const network::GameClient& client, Logger& logger)
{
	switch (client.status()) {
		case network::ClientStatus::Connected:
			logger.info("Joined as player " + std::to_string(client.playerId()));
			return true;
		case network::ClientStatus::Rejected:
			logger.error("Server rejected the join request");
			return false;
		case network::ClientStatus::TimedOut:
			logger.error("Join timed out");
			return false;
		case network::ClientStatus::Disconnected:
			logger.error("The server closed this session");
			return false;
		default:
			return true;
	}
}

/// Writes any server notices received during the latest network update.
static void reportNotices(network::GameClient& client, Logger& logger)
{
	if (const auto departure = client.takeDeparture()) {
		logger.info("Player " + std::to_string(departure->playerId) + " left");
	}
	if (const auto error = client.takeError()) {
		logger.warn("Server protocol error " + std::to_string(static_cast<int>(*error)));
	}
}

/// Polls one frame and sends the current input state.
static bool updateFrame(Sfml& window, network::GameClient& client, Logger& logger, network::ClientStatus& previous)
{
	window.handleEvent();
	if (!window.isOpen()) {
		return true;
	}
	const std::uint8_t buttons = window.buttons();
	client.tick(buttons);
	reportNotices(client, logger);
	if (client.status() != previous) {
		previous = client.status();
		if (!reportStatus(client, logger)) {
			return false;
		}
	}
	window.displayWindow(client.worldSnapshot());
	return true;
}

/// Runs the graphical client until the window closes or a signal arrives.
int runClient(const ClientOptions& options)
{
	std::signal(SIGINT, stopClient);
	std::signal(SIGTERM, stopClient);
	auto logger = makeLogger(options.logFile);
	logger->setVerbose(true);
	AssetManager assets;
	assets.preload(sprites::PLAYER_SHEET);
	network::GameClient client(options.host, options.port);
	Sfml window(assets);
	auto previous = client.status();
	logger->info("Connecting to " + options.host + ':' + std::to_string(options.port));
	while (running != 0 && window.isOpen()) {
		if (!updateFrame(window, client, *logger, previous)) {
			return 1;
		}
	}
	client.disconnect();
	logger->info("Disconnected");
	return 0;
}

}
