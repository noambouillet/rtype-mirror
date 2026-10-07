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

}
