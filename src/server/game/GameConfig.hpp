/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Gameplay tuning values shared by the server world, prefabs and systems
*/

#pragma once

#include <cstddef>

namespace rtype::config {

constexpr std::size_t MAX_PLAYERS = 4;

constexpr float WORLD_WIDTH = 1280.0f;
constexpr float WORLD_HEIGHT = 720.0f;
constexpr float OFFSCREEN_MARGIN = 64.0f;

constexpr float PLAYER_SPEED = 250.0f;
constexpr int PLAYER_HEALTH = 3;
constexpr float PLAYER_WIDTH = 64.0f;
constexpr float PLAYER_HEIGHT = 32.0f;
constexpr float PLAYER_START_X = 100.0f;
constexpr float PLAYER_START_Y = 100.0f;
constexpr float PLAYER_START_SPACING = 50.0f;
constexpr float FIRE_COOLDOWN = 0.25f;

constexpr float PROJECTILE_SPEED = 600.0f;
constexpr float PROJECTILE_WIDTH = 32.0f;
constexpr float PROJECTILE_HEIGHT = 16.0f;
constexpr int PROJECTILE_DAMAGE = 1;

constexpr float ENEMY_SPEED = 150.0f;
constexpr int ENEMY_HEALTH = 1;
constexpr int ENEMY_CONTACT_DAMAGE = 1;
constexpr float ENEMY_WIDTH = 42.0f;
constexpr float ENEMY_HEIGHT = 48.0f;

constexpr float WAVE_INTERVAL = 2.5f;
constexpr int WAVE_MIN_SIZE = 2;
constexpr int WAVE_MAX_SIZE = 4;

}
