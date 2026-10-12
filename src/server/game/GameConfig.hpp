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
constexpr float OFFSCREEN_MARGIN = 96.0f;
constexpr float GROUND_Y = WORLD_HEIGHT;

constexpr float PLAYER_SPEED = 250.0f;
constexpr int PLAYER_HEALTH = 3;
constexpr float PLAYER_WIDTH = 96.0f;
constexpr float PLAYER_HEIGHT = 48.0f;
constexpr float PLAYER_START_X = 100.0f;
constexpr float PLAYER_START_Y = 100.0f;
constexpr float PLAYER_START_SPACING = 75.0f;
constexpr float FIRE_COOLDOWN = 0.25f;

constexpr float PROJECTILE_SPEED = 600.0f;
constexpr float PROJECTILE_WIDTH = 32.0f;
constexpr float PROJECTILE_HEIGHT = 16.0f;
constexpr int PROJECTILE_DAMAGE = 1;

constexpr float ENEMY_PROJECTILE_SPEED = 280.0f;
constexpr float ENEMY_PROJECTILE_WIDTH = 14.0f;
constexpr float ENEMY_PROJECTILE_HEIGHT = 12.0f;
constexpr int ENEMY_PROJECTILE_DAMAGE = 1;
constexpr float ENEMY_FIRST_SHOT_MIN = 0.5f;
constexpr float ENEMY_MIN_FIRE_DISTANCE = 150.0f;

constexpr int ENEMY_CONTACT_DAMAGE = 1;

constexpr float PATAPATA_SPEED = 420.0f;
constexpr float PATAPATA_AMPLITUDE = 40.0f;
constexpr float PATAPATA_FREQUENCY = 3.0f;
constexpr float PATAPATA_DELAY = 0.30f;
constexpr float PATAPATA_SPREAD = 20.0f;
constexpr float PATAPATA_WIDTH = 63.0f;
constexpr float PATAPATA_HEIGHT = 72.0f;
constexpr int PATAPATA_HEALTH = 1;
constexpr int PATAPATA_MIN_COUNT = 4;
constexpr int PATAPATA_MAX_COUNT = 6;
constexpr float PATAPATA_FIRE_COOLDOWN = 0.5f;

constexpr float BUG_SPEED = 420.0f;
constexpr float BUG_DELAY = 0.30f;
constexpr float BUG_WIDTH = 72.0f;
constexpr float BUG_HEIGHT = 66.0f;
constexpr int BUG_HEALTH = 1;
constexpr int BUG_MIN_COUNT = 5;
constexpr int BUG_MAX_COUNT = 8;
constexpr float BUG_FIRE_COOLDOWN = 2.5f;
constexpr float BUG_ARC_HEIGHT = 200.0f;
constexpr float BUG_ARC_TURNS = 1.25f;

constexpr float BINK_SPEED = 120.0f;
constexpr float BINK_WIDTH = 78.0f;
constexpr float BINK_HEIGHT = 78.0f;
constexpr int BINK_HEALTH = 1;
constexpr float BINK_FIRE_COOLDOWN = 2.0f;
constexpr float BINK_WAVE_CHANCE = 0.25f;

constexpr float SCANT_SPEED = 200.0f;
constexpr float SCANT_HOLD_X = WORLD_WIDTH - 200.0f;
constexpr float SCANT_TRACK_SPEED = 120.0f;
constexpr float SCANT_STAY_TIME = 10.0f;
constexpr float SCANT_FIRE_COOLDOWN = 2.5f;
constexpr float SCANT_CHARGE_TIME = 0.6f;
constexpr float SCANT_WIDTH = 135.0f;
constexpr float SCANT_HEIGHT = 135.0f;
constexpr float SCANT_CANNON_Y = -27.0f;
constexpr int SCANT_HEALTH = 5;
constexpr float SCANT_WAVE_CHANCE = 0.15f;

constexpr float SCANT_BEAM_SPEED = 500.0f;
constexpr float SCANT_BEAM_WIDTH = 90.0f;
constexpr float SCANT_BEAM_HEIGHT = 24.0f;
constexpr int SCANT_BEAM_DAMAGE = 1;

constexpr float WAVE_INTERVAL = 3.5f;

}
