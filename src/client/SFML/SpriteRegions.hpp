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
static constexpr sf::IntRect PLAYER_SHOT = {{249, 105}, {16, 8}};

static constexpr std::string_view ENEMY_SHOT_SHEET = "r-typesheet43.gif";
static constexpr sf::IntRect ENEMY_SHOT = {{136, 6}, {7, 6}};

static constexpr std::string_view SCANT_SHEET = "r-typesheet9.gif";
static constexpr sf::IntRect ENEMY_SCANT = {{52, 3}, {56, 54}};
static constexpr sf::IntRect SCANT_BEAM = {{36, 59}, {54, 16}};

static constexpr std::string_view BINK_SHEET = "r-typesheet7.gif";
static constexpr sf::IntRect ENEMY_BINK = {{1, 4}, {32, 29}};

}
