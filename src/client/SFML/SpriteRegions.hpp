/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Spritesheet names and pixel regions UwU
*/

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string_view>

namespace rtype::sprites {

static constexpr std::string_view PLAYER_SHEET = "r-typesheet1.gif";
static constexpr sf::IntRect PLAYER_SHIP{{100, 1}, {32, 17}};

}
