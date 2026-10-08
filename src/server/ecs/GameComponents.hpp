/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** R-Type gameplay components (server side)
*/

#pragma once

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

struct Weapon {
	float cooldown = 0.0f;
	float remaining = 0.0f;
	bool firing = false;
};

struct WaveSpawner {
	float interval = 0.0f;
	float timer = 0.0f;
	int minSize = 0;
	int maxSize = 0;
};

struct DestroyOffscreen {};

struct DestroyOnHit {};

}
