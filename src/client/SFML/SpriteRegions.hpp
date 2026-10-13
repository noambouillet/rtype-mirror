/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Spritesheet names and pixel regions UwU
*/

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <array>
#include <string_view>

namespace rtype::sprites {

static constexpr std::string_view PLAYER_SHEET = "r-typesheet42.gif";
static constexpr std::array<sf::IntRect, 4> PLAYER_SHIPS = {
	sf::IntRect{{1, 69}, {32, 16}},  // Blue
	sf::IntRect{{1, 52}, {32, 16}},  // Red
	sf::IntRect{{1, 35}, {32, 16}},  // Green
	sf::IntRect{{1, 18}, {32, 16}},  // Magenta
};

static constexpr std::string_view ENEMY_SHEET = "r-typesheet5.gif";
static constexpr sf::IntRect ENEMY_PATAPATA = {{0, 0}, {33, 36}};

static constexpr std::string_view BUG_SHEET = "r-typesheet8.gif";
static constexpr sf::IntRect ENEMY_BUG = {{0, 0}, {33, 34}};

static constexpr std::string_view SHOT_SHEET = "r-typesheet1.gif";
static constexpr std::array<sf::IntRect, 4> PLAYER_SHOT_FRAMES = {
	sf::IntRect{{232, 103}, {16, 12}},
	sf::IntRect{{249, 103}, {16, 12}},
	sf::IntRect{{267, 103}, {16, 12}},
	sf::IntRect{{284, 103}, {16, 12}},
};
static constexpr std::array<sf::IntRect, 9> SHOT_BURST_FRAMES = {
	sf::IntRect{{199, 83}, {16, 16}},
	sf::IntRect{{215, 83}, {16, 16}},
	sf::IntRect{{231, 83}, {16, 16}},
	sf::IntRect{{247, 83}, {16, 16}},
	sf::IntRect{{263, 83}, {16, 16}},
	sf::IntRect{{279, 83}, {16, 16}},
	sf::IntRect{{295, 83}, {16, 16}},
	sf::IntRect{{311, 83}, {16, 16}},
	sf::IntRect{{327, 83}, {16, 16}},
};
static constexpr std::array<sf::IntRect, 7> HIT_SPARK_FRAMES = {
	sf::IntRect{{215, 83}, {16, 16}},
	sf::IntRect{{231, 83}, {16, 16}},
	sf::IntRect{{247, 83}, {16, 16}},
	sf::IntRect{{263, 83}, {16, 16}},
	sf::IntRect{{279, 83}, {16, 16}},
	sf::IntRect{{295, 83}, {16, 16}},
	sf::IntRect{{311, 83}, {16, 16}},
};

static constexpr std::string_view EXPLOSION_SHEET = "r-typesheet44.gif";
static constexpr std::array<int, 12> MOB_EXPLOSION_X = {129, 162, 195, 228, 261, 294, 327, 360, 393, 426, 459, 492};
static constexpr std::array<sf::IntRect, 12> MOB_EXPLOSION_FRAMES = [] {
	std::array<sf::IntRect, 12> frames{};
	for (std::size_t index = 0; index < frames.size(); index++) {
		frames[index] = sf::IntRect{{MOB_EXPLOSION_X[index], 0}, {33, 33}};
	}
	return frames;
}();
static constexpr std::array<int, 10> PLAYER_EXPLOSION_X = {1, 66, 131, 196, 261, 327, 392, 457, 522, 587};
static constexpr std::array<sf::IntRect, 10> PLAYER_EXPLOSION_FRAMES = [] {
	std::array<sf::IntRect, 10> frames{};
	for (std::size_t index = 0; index < frames.size(); index++) {
		frames[index] = sf::IntRect{{PLAYER_EXPLOSION_X[index], 35}, {64, 64}};
	}
	return frames;
}();

static constexpr std::string_view ENEMY_SHOT_SHEET = "r-typesheet43.gif";
static constexpr sf::IntRect ENEMY_SHOT = {{136, 6}, {7, 6}};

static constexpr std::string_view SCANT_SHEET = "r-typesheet9.gif";
static constexpr sf::IntRect ENEMY_SCANT = {{52, 3}, {56, 54}};
static constexpr sf::IntRect SCANT_BEAM = {{36, 59}, {54, 16}};

static constexpr std::string_view BINK_SHEET = "r-typesheet7.gif";
static constexpr sf::IntRect ENEMY_BINK = {{1, 4}, {32, 29}};

}
