/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Client presentation events
*/

#pragma once

#include <cstdint>
#include <variant>

#include "game/GameTypes.hpp"

namespace rtype::client {

struct SpawnEvent {
	std::uint32_t id;
	game::EntityType kind;
	std::uint8_t owner;
	float x;
	float y;
};

struct DestroyEvent {
	std::uint32_t id;
	game::EntityType kind;
	float x;
	float y;
};

struct DamageEvent {
	std::uint32_t id;
};

struct DeathEvent {
	std::uint32_t id;
	float x;
	float y;
};

using ViewEvent = std::variant<SpawnEvent, DestroyEvent, DamageEvent, DeathEvent>;

}
