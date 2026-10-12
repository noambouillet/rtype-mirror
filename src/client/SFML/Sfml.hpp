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
#include <optional>
#include <vector>

#include "AssetManager.hpp"
#include "VisualEffects.hpp"

namespace rtype {
namespace network {
struct WorldEntity;
struct WorldEventNotice;
struct WorldSnapshot;
}

class Sfml {
private:
	static constexpr float JOYSTICK_THRESHOLD = 50.0F;
	static constexpr auto SHOT_FRAME_TIME = std::chrono::milliseconds(80);

	std::uint8_t keyboardButtons() const;
	std::uint8_t joystickButtons() const;
	void drawPlayer(const network::WorldEntity& entity);
	void drawPlayerShot(const network::WorldEntity& entity);
	void drawEnemy(const network::WorldEntity& entity);

	sf::RenderWindow _window;
	AssetManager& _assets;
	VisualEffects _visualEffects;
	unsigned int _joystickId;
	std::chrono::steady_clock::time_point _animationStart;

public:
	explicit Sfml(AssetManager& assets);
	bool isOpen() const;
	void handleEvent();
	std::uint8_t buttons() const;
	void predictLocalFire(std::uint8_t buttons, std::optional<std::uint8_t> localPlayerId, const std::optional<network::WorldSnapshot>& snapshot);
	void displayWindow(const std::optional<network::WorldSnapshot>& snapshot, const std::vector<network::WorldEventNotice>& events);
};

}
