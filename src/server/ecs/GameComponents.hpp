/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** R-Type gameplay components (server side)
*/

#pragma once

#include <cstddef>
#include <cstdint>

#include "utils/network/Protocol.hpp"

namespace rtype::ecs {

//type of entity to handle collision detection with tag/mask
namespace layer {
	constexpr std::uint32_t PLAYER = 0b0001;
	constexpr std::uint32_t ENEMY = 0b0010;
	constexpr std::uint32_t PLAYER_PROJECTILE = 0b0100;
	constexpr std::uint32_t ENEMY_PROJECTILE = 0b1000;
}

struct Replicated {
	network::EntityType type = network::EntityType::Enemy;
	std::uint8_t owner = network::NO_OWNER;
	std::uint8_t variant = 0;
};

struct PlayerControl {
	std::uint8_t playerId = 0;
	float speed = 0.0f;
};

/// What an enemy weapon fires; player ships always shoot straight ahead.
enum class ShotKind : std::uint8_t {
	Aimed,  // an orb toward the closest player
	Beam,  // a charge-shot beam straight left, from the cannon
};

struct Weapon {
	float cooldown = 0.0f;
	float remaining = 0.0f;
	bool firing = false;
	ShotKind shot = ShotKind::Aimed;
};

struct WaveSpawner {
	float interval = 0.0f;
	float timer = 0.0f;
};

enum class OffscreenAction : std::uint8_t {
	Destroy,
	Block,
};

struct Offscreen {
	OffscreenAction action = OffscreenAction::Destroy;
};

struct DestroyOnHit {};

enum class EnemyKind : std::uint8_t {
	Patapata = 0,
	Bug = 1,
	Scant = 2,
	Bink = 3,
};

struct SineMotion {
	float speed = 0.0f;
	float amplitude = 0.0f;
	float frequency = 0.0f;
	float phase = 0.0f;
	float elapsed = 0.0f;
};

struct FormationSpawner {
	EnemyKind kind = EnemyKind::Patapata;
	int remaining = 0;
	float delay = 0.0f;
	float timer = 0.0f;
	float y = 0.0f;
	float spread = 0.0f;
};

enum class PathId : std::uint8_t {
	Arc,
};

struct PathFollow {
	PathId path = PathId::Arc;
	std::size_t next = 0;
	float speed = 0.0f;
	float originX = 0.0f;
	float originY = 0.0f;
};

struct HoverMotion {
	float speed = 0.0f;
	float holdX = 0.0f;
	float trackSpeed = 0.0f;
	float stayTime = 0.0f;
	float chargeTime = 0.0f;
	float elapsed = 0.0f;
};

}
