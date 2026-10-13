/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Client-side scrolling background
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string_view>

#include "AssetManager.hpp"

namespace rtype {

class ScrollingBackground {
private:
	static constexpr float SCROLL_SPEED = 40.0F;
	static constexpr std::string_view TEXTURE_NAME = "background.png";

	sf::RenderWindow& _window;
	const sf::Texture& _texture;
	sf::Sprite _sprite;
	float _offset;
	sf::Vector2u _windowSize;

	void updateTextureRect();

public:
	ScrollingBackground(AssetManager& assets, sf::RenderWindow& window);
	void update(float elapsedTime);
	void draw(sf::RenderTarget& target) const;
};

}
