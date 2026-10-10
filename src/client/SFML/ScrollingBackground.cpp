/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Client-side scrolling background
*/

#include "ScrollingBackground.hpp"

#include <cmath>

namespace rtype {

ScrollingBackground::ScrollingBackground(AssetManager& assets, sf::RenderWindow& window)
	: _window(window),
	  _texture(assets.texture("background.png")),
	  _sprite(_texture),
	  _offset(0.0F),
	  _windowSize(window.getSize())
{
	assets.setRepeated(TEXTURE_NAME, true);
	updateTextureRect();
}

/// Updates the background offset based on the elapsed time since the last frame.
void ScrollingBackground::update(float elapsedTime)
{
	if (elapsedTime > 0.0F) {
		const float textureWidth = static_cast<float>(_texture.getSize().x);
		_offset = std::fmod(_offset + SCROLL_SPEED * elapsedTime, textureWidth);
	}
	const sf::Vector2u size = _window.getSize();
	_windowSize = size;
	updateTextureRect();
}

/// Draws the repeated texture across the window behind game entities.
void ScrollingBackground::draw(sf::RenderTarget& target) const
{
	if (_windowSize.x > 0 && _windowSize.y > 0) {
		target.draw(_sprite);
	}
}

/// Updates the texture rectangle and sprite position to create a scrolling effect.
void ScrollingBackground::updateTextureRect()
{
	if (_windowSize.x == 0 || _windowSize.y == 0) {
		return;
	}
	const int left = static_cast<int>(_offset);
	const float fractionalOffset = _offset - static_cast<float>(left);
	const int width = static_cast<int>(_windowSize.x) + 1;
	const int height = static_cast<int>(_windowSize.y);
	_sprite.setTextureRect(sf::IntRect({left, 0}, {width, height}));
	_sprite.setPosition({-fractionalOffset, 0.0F});
}

}
