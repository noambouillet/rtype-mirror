/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** SFML client window implementation
*/

#include "Sfml.hpp"

#include <algorithm>
#include <cmath>

#include "utils/RTypeError.hpp"

namespace rtype {

/// Opens the window and loads the current placeholder player sprite.
Sfml::Sfml() : _window(sf::VideoMode({1280, 720}), "R-Type"), _playerSprite(_playerTexture), _joystickId(0), _lastFrame(std::chrono::steady_clock::now())
{
	_window.setFramerateLimit(60);
	if (!_playerTexture.loadFromFile("assets/freaky-apple.jpg")) {
		throw GraphicsError("Cannot load assets/freaky-apple.jpg");
	}
	_playerSprite.setTexture(_playerTexture, true);
	_playerSprite.setScale({0.5, 0.5});
	_playerSprite.setPosition({100.0, 200.0});
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

/// Moves the temporary sprite locally while server world state is unavailable.
void Sfml::movePlayer(std::uint8_t buttons)
{
	const auto now = std::chrono::steady_clock::now();
	const float elapsed = std::min(std::chrono::duration<float>(now - _lastFrame).count(), MAX_FRAME_SECONDS);
	_lastFrame = now;
	const float horizontal = static_cast<float>((buttons & BUTTON_RIGHT) != 0) - static_cast<float>((buttons & BUTTON_LEFT) != 0);
	const float vertical = static_cast<float>((buttons & BUTTON_DOWN) != 0) - static_cast<float>((buttons & BUTTON_UP) != 0);
	const float length = std::sqrt(horizontal * horizontal + vertical * vertical);
	if (length == 0) {
		return;
	}
	auto position = _playerSprite.getPosition();
	const float distance = PLAYER_SPEED * elapsed / length;
	position.x += horizontal * distance;
	position.y += vertical * distance;
	const auto bounds = _playerSprite.getGlobalBounds();
	position.x = std::clamp(position.x, 0.0f, std::max(0.0f, static_cast<float>(_window.getSize().x) - bounds.size.x));
	position.y = std::clamp(position.y, 0.0f, std::max(0.0f, static_cast<float>(_window.getSize().y) - bounds.size.y));
	_playerSprite.setPosition(position);
}

/// Draws the temporary sprite while world snapshots are unimplemented.
void Sfml::displayWindow()
{
	_window.clear(sf::Color::Black);
	_window.draw(_playerSprite);
	_window.display();
}

}
