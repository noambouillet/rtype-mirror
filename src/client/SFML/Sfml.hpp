/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdint>

#include "AssetManager.hpp"

namespace rtype {
namespace network {
struct WorldEntity;
struct WorldSnapshot;
}

class Sfml {
private:
	static constexpr float JOYSTICK_THRESHOLD = 50;
	static constexpr float PLAYER_SPEED = 250;
	static constexpr float MAX_FRAME_SECONDS = 0.1;
	static constexpr std::uint32_t WINDOW_WIDTH = 1280;
	static constexpr std::uint32_t WINDOW_HEIGHT = 720;

	std::uint8_t keyboardButtons() const;
	std::uint8_t joystickButtons() const;
	void drawPlayer(const network::WorldEntity& entity);
	void drawPlayerShot(const network::WorldEntity& entity);
	void drawEnemy(const network::WorldEntity& entity);

	sf::RenderWindow _window;
	AssetManager& _assets;
	unsigned int _joystickId;
	std::chrono::steady_clock::time_point _lastFrame;

public:
	explicit Sfml(AssetManager& assets);
	bool isOpen() const;
	void handleEvent();
	std::uint8_t buttons() const;
	void displayWindow(const std::optional<network::WorldSnapshot>& snapshot);
};

}
