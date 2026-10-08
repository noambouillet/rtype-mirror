/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Components
*/

#pragma once
#include <cstdint>
#include <string>

namespace rtype::ecs {

struct Position {
	float x = 0.0f;
	float y = 0.0f;
};

struct Velocity {
	float x = 0.0f;
	float y = 0.0f;
};

struct Health {
	int maxHP = 0;
	int hp = 0;
};

struct Damage {
	int amount = 0;
};

struct Sprite {
	std::string textureName;
	float scale = 1.0f;
	float rotation = 0.0f;
	bool isUI = false;
};

struct Destroy {};

struct Collider {
	float width = 0.0f;
	float height = 0.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	std::uint32_t tag = 0;
	std::uint32_t mask = 0;
};

}
