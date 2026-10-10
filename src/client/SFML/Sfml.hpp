/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

#include "AssetManager.hpp"

namespace rtype {
namespace network {
struct WorldEntity;
struct WorldEventNotice;
struct WorldSnapshot;
}

class Sfml {
private:
	static constexpr float JOYSTICK_THRESHOLD = 50.0F;
	static constexpr float PLAYER_SPEED = 250.0F;
	static constexpr float MAX_FRAME_SECONDS = 0.1F;
	static constexpr std::uint32_t WINDOW_WIDTH = 1280;
	static constexpr std::uint32_t WINDOW_HEIGHT = 720;
	static constexpr std::size_t MAX_EFFECTS = 128;
	static constexpr auto SHOT_FRAME_TIME = std::chrono::milliseconds(80);
	static constexpr auto BURST_FRAME_TIME = std::chrono::milliseconds(55);
	static constexpr auto IMPACT_FRAME_TIME = std::chrono::milliseconds(40);
	static constexpr auto MOB_EXPLOSION_FRAME_TIME = std::chrono::milliseconds(25);
	static constexpr auto PLAYER_EXPLOSION_FRAME_TIME = std::chrono::milliseconds(70);

	enum class EffectKind : std::uint8_t { ShotBurst,
		HitSpark,
		MobExplosion,
		PlayerExplosion };

	struct VisualEffect {
		std::uint32_t entityId;
		EffectKind kind;
		sf::Vector2f position;
		std::chrono::steady_clock::time_point started;
	};

	std::uint8_t keyboardButtons() const;
	std::uint8_t joystickButtons() const;
	void drawPlayer(const network::WorldEntity& entity);
	void drawPlayerShot(const network::WorldEntity& entity);
	void drawEnemy(const network::WorldEntity& entity);
	void displayEvents(const std::vector<network::WorldEventNotice>& events, const std::optional<network::WorldSnapshot>& snapshot);
	void addEffect(std::uint32_t entityId, EffectKind kind, sf::Vector2f position);
	void drawEffect(const VisualEffect& effect, std::chrono::steady_clock::time_point now);
	static std::chrono::milliseconds effectDuration(EffectKind kind);

	sf::RenderWindow _window;
	AssetManager& _assets;
	unsigned int _joystickId;
	std::chrono::steady_clock::time_point _animationStart;
	std::optional<std::uint64_t> _positionsTick;
	std::unordered_map<std::uint32_t, sf::Vector2f> _entityPositions;
	std::vector<VisualEffect> _effects;

public:
	explicit Sfml(AssetManager& assets);
	bool isOpen() const;
	void handleEvent();
	std::uint8_t buttons() const;
	void predictLocalFire(std::uint8_t buttons, std::optional<std::uint8_t> localPlayerId, const std::optional<network::WorldSnapshot>& snapshot);
	void displayWindow(const std::optional<network::WorldSnapshot>& snapshot, const std::vector<network::WorldEventNotice>& events);
};

}
