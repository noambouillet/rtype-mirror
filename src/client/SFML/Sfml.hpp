/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>

namespace rtype {
namespace network {
struct WorldSnapshot;
}

class Sfml {
private:
	static constexpr float JOYSTICK_THRESHOLD = 50;
	static constexpr float PLAYER_SPEED = 250;
	static constexpr float MAX_FRAME_SECONDS = 0.1;
	static constexpr std::uint8_t BUTTON_UP = 0x01;
	static constexpr std::uint8_t BUTTON_DOWN = 0x02;
	static constexpr std::uint8_t BUTTON_LEFT = 0x04;
	static constexpr std::uint8_t BUTTON_RIGHT = 0x08;
	static constexpr std::uint8_t BUTTON_FIRE = 0x10;
	static constexpr std::uint32_t WINDOW_WIDTH = 1280;
	static constexpr std::uint32_t WINDOW_HEIGHT = 720;

	std::uint8_t keyboardButtons() const;
	std::uint8_t joystickButtons() const;

	sf::RenderWindow _window;
	std::array<sf::Texture, 4> _playerTextures;
	unsigned int _joystickId;
	std::chrono::steady_clock::time_point _lastFrame;

public:
	Sfml();
	bool isOpen() const;
	void handleEvent();
	std::uint8_t buttons() const;
	void displayWindow(const std::optional<network::WorldSnapshot>& snapshot);
};

}
