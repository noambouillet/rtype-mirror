/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Entity prefabs: one function per kind of game entity
*/

#pragma once

#include <cstdint>

#include "Components.hpp"
#include "Coordinator.hpp"

namespace rtype::prefabs {

bool canSpawnReplicated(ecs::Coordinator& coordinator);
ecs::Entity createPlayer(ecs::Coordinator& coordinator, std::uint8_t playerId);
ecs::Entity createPlayerProjectile(ecs::Coordinator& coordinator, std::uint8_t playerId, ecs::Position origin);
ecs::Entity createEnemy(ecs::Coordinator& coordinator, float y);
ecs::Entity createWaveSpawner(ecs::Coordinator& coordinator);

}
