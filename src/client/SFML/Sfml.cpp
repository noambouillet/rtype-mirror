/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** SFML client window implementation
*/

#include "Sfml.hpp"

#include "client/network/GameClient.hpp"
#include "utils/RTypeError.hpp"

namespace rtype {

/// Opens the window and loads the current placeholder player sprite.
Sfml::Sfml() : _window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "R-Type"), _joystickId(0), _lastFrame(std::chrono::steady_clock::now())
{
	_window.setFramerateLimit(60);
	std::string assetPaths[4] = {
		"assets/freaky-apple.jpg",
		"assets/kirk.jpg",
		"assets/hannah.jpg",
		"assets/scuba.jpg"};

	for (int i = 0; i < 4; i++) {
		if (!_playerTextures[i].loadFromFile(assetPaths[i])) {
			throw GraphicsError("Cannot load " + assetPaths[i]);
		}
	}
}

bool Sfml::isOpen() const
{
	return _window.isOpen();
}

/// Closes the window on close or Escape events.
void Sfml::handleEvent()
{
	while (const auto event = _window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			_window.close();
		}
		if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
			if (key->code == sf::Keyboard::Key::Escape) {
				_window.close();
			}
		}
	}
}

/// Combines keyboard and joystick buttons into the protocol bit field.
std::uint8_t Sfml::buttons() const
{
	return keyboardButtons() | joystickButtons();
}

/// Reads the keyboard's current input state.
std::uint8_t Sfml::keyboardButtons() const
{
	if (!_window.hasFocus()) {
		return 0;
	}
	std::uint8_t value = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		value |= BUTTON_UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		value |= BUTTON_DOWN;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		value |= BUTTON_LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		value |= BUTTON_RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		value |= BUTTON_FIRE;
	}
	return value;
}

/// Reads the first joystick when it is connected.
std::uint8_t Sfml::joystickButtons() const
{
	if (!sf::Joystick::isConnected(_joystickId)) {
		return 0;
	}
	std::uint8_t value = 0;
	const float horizontal = sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::X);
	const float vertical = sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::Y);
	if (vertical < -JOYSTICK_THRESHOLD) {
		value |= BUTTON_UP;
	}
	if (vertical > JOYSTICK_THRESHOLD) {
		value |= BUTTON_DOWN;
	}
	if (horizontal < -JOYSTICK_THRESHOLD) {
		value |= BUTTON_LEFT;
	}
	if (horizontal > JOYSTICK_THRESHOLD) {
		value |= BUTTON_RIGHT;
	}
	if (sf::Joystick::isButtonPressed(_joystickId, 0)) {
		value |= BUTTON_FIRE;
	}
	return value;
}

/// Draws every entities from the snapshot, or a black screen if not yet connected
void Sfml::displayWindow(const std::optional<network::WorldSnapshot>& snapshot)
{
	_window.clear(sf::Color::Black);
	if (snapshot) {
		for (const auto& entity : snapshot->entities) {
			if (entity.entityType == network::EntityType::Player) {
				std::uint8_t safeSpriteId = entity.spriteId;
				if (safeSpriteId >= _playerTextures.size())
					safeSpriteId = 0;
				sf::Sprite sprite(_playerTextures[safeSpriteId]);
				sprite.setScale({0.5f, 0.5f});
				sprite.setPosition({entity.x, entity.y});
				_window.draw(sprite);
			}
		}
	}
	_window.display();
}

}
