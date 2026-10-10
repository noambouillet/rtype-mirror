/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** SFML client window implementation
*/

#include "Sfml.hpp"

#include "SpriteRegions.hpp"
#include "client/GameConfig.hpp"
#include "client/network/GameClient.hpp"

namespace rtype {

/// Opens the window and prepares the game using the AssetManager
Sfml::Sfml(AssetManager& assets)
	: _window(sf::VideoMode({client_config::WINDOW_WIDTH, client_config::WINDOW_HEIGHT}), "R-Type"),
	  _assets(assets),
	  _visualEffects(assets),
	  _joystickId(0),
	  _animationStart(std::chrono::steady_clock::now())
{
	_window.setFramerateLimit(60);
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

/// Plays the local weapon effect before waiting for the server's projectile spawn.
void Sfml::predictLocalFire(std::uint8_t buttons, std::optional<std::uint8_t> localPlayerId, const std::optional<network::WorldSnapshot>& snapshot)
{
	_visualEffects.predictLocalFire(buttons, localPlayerId, snapshot);
}

/// Reads the keyboard's current input state.
std::uint8_t Sfml::keyboardButtons() const
{
	if (!_window.hasFocus()) {
		return 0;
	}
	std::uint8_t value = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		value |= network::BUTTON_UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		value |= network::BUTTON_DOWN;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		value |= network::BUTTON_LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		value |= network::BUTTON_RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		value |= network::BUTTON_FIRE;
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
		value |= network::BUTTON_UP;
	}
	if (vertical > JOYSTICK_THRESHOLD) {
		value |= network::BUTTON_DOWN;
	}
	if (horizontal < -JOYSTICK_THRESHOLD) {
		value |= network::BUTTON_LEFT;
	}
	if (horizontal > JOYSTICK_THRESHOLD) {
		value |= network::BUTTON_RIGHT;
	}
	if (sf::Joystick::isButtonPressed(_joystickId, 0)) {
		value |= network::BUTTON_FIRE;
	}
	return value;
}

/// Draws a player ship, tinted by its player ID.
void Sfml::drawPlayer(const network::WorldEntity& entity)
{
	std::uint8_t safeSpriteId = entity.spriteId;
	if (safeSpriteId >= sprites::PLAYER_SHIPS.size()) {
		safeSpriteId = 0;
	}
	sf::Sprite sprite = _assets.sprite(sprites::PLAYER_SHEET, sprites::PLAYER_SHIPS[safeSpriteId]);
	sprite.setOrigin({16.0F, 8.0F});
	sprite.setScale({2.0F, 2.0F});
	sprite.setPosition({entity.x, entity.y});
	_window.draw(sprite);
}

/// Draws a projectile fired by a player.
void Sfml::drawPlayerShot(const network::WorldEntity& entity)
{
	const std::chrono::milliseconds age = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _animationStart);
	const std::size_t frameIndex = static_cast<std::size_t>(age.count() / SHOT_FRAME_TIME.count()) % sprites::PLAYER_SHOT_FRAMES.size();
	sf::Sprite sprite = _assets.sprite(sprites::SHOT_SHEET, sprites::PLAYER_SHOT_FRAMES[frameIndex]);
	sprite.setOrigin({8.0F, 6.0F});
	sprite.setScale({2.0F, 2.0F});
	sprite.setPosition({entity.x, entity.y});
	_window.draw(sprite);
}

/// Draws a Bydo enemy.
void Sfml::drawEnemy(const network::WorldEntity& entity)
{
	sf::Sprite sprite = _assets.sprite(sprites::ENEMY_SHEET, sprites::ENEMY_BYDO);
	sprite.setOrigin({16.5F, 18.0F});
	sprite.setScale({2.0F, 2.0F});
	sprite.setPosition({entity.x, entity.y});
	_window.draw(sprite);
}

/// Draws the latest snapshot and temporary sprite animations in one frame.
void Sfml::displayWindow(const std::optional<network::WorldSnapshot>& snapshot, const std::vector<network::WorldEventNotice>& events)
{
	_window.clear(sf::Color::Black);
	if (snapshot) {
		for (const auto& entity : snapshot->entities) {
			switch (entity.entityType) {
				case network::EntityType::Player:
					drawPlayer(entity);
					break;
				case network::EntityType::PlayerProjectile:
					drawPlayerShot(entity);
					break;
				case network::EntityType::Enemy:
					drawEnemy(entity);
					break;
				default:
					break;
			}
		}
	}
	_visualEffects.display(_window, snapshot, events);
	_window.display();
}

}
