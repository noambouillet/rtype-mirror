/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** SFML client window implementation
*/

#include "Sfml.hpp"

#include <algorithm>
#include <optional>
#include <span>
#include <string_view>
#include <variant>

#include "SpriteRegions.hpp"
#include "client/network/GameClient.hpp"

namespace rtype {

/// Draws one frame from a spritesheet animation at a world position.
static void drawAnimation(sf::RenderWindow& window, AssetManager& assets, std::string_view sheet,
	std::span<const sf::IntRect> frames, sf::Vector2f position, std::chrono::milliseconds age,
	std::chrono::milliseconds frameTime, float scale)
{
	const auto index = static_cast<std::size_t>(age.count() / frameTime.count());
	if (index >= frames.size()) {
		return;
	}
	const sf::IntRect& frame = frames[index];
	sf::Sprite sprite = assets.sprite(sheet, frame);
	sprite.setOrigin({static_cast<float>(frame.size.x) / 2.0F, static_cast<float>(frame.size.y) / 2.0F});
	sprite.setScale({scale, scale});
	sprite.setPosition(position);
	window.draw(sprite);
}

/// Opens the window and prepares the game using the AssetManager
Sfml::Sfml(AssetManager& assets)
	: _window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "R-Type"),
	  _assets(assets),
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

/// Returns the lifetime of a spritesheet animation.
std::chrono::milliseconds Sfml::effectDuration(EffectKind kind)
{
	switch (kind) {
		case EffectKind::ShotBurst:
			return BURST_FRAME_TIME * sprites::SHOT_BURST_FRAMES.size();
		case EffectKind::HitSpark:
			return IMPACT_FRAME_TIME * sprites::HIT_SPARK_FRAMES.size();
		case EffectKind::MobExplosion:
			return MOB_EXPLOSION_FRAME_TIME * sprites::MOB_EXPLOSION_FRAMES.size();
		case EffectKind::PlayerExplosion:
			return PLAYER_EXPLOSION_FRAME_TIME * sprites::PLAYER_EXPLOSION_FRAMES.size();
	}
	return std::chrono::milliseconds(0);
}

/// Starts one bounded animation per entity and effect kind.
void Sfml::addEffect(std::uint32_t entityId, EffectKind kind, sf::Vector2f position)
{
	for (VisualEffect& effect : _effects) {
		if (effect.entityId == entityId && effect.kind == kind) {
			effect.position = position;
			return;
		}
	}
	if (_effects.size() == MAX_EFFECTS) {
		_effects.erase(_effects.begin());
	}
	_effects.push_back(VisualEffect{.entityId = entityId,
		.kind = kind,
		.position = position,
		.started = std::chrono::steady_clock::now()});
}

/// Draws the current sprite frame of an effect without changing world state.
void Sfml::drawEffect(const VisualEffect& effect, std::chrono::steady_clock::time_point now)
{
	const auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now - effect.started);
	sf::Vector2f position = effect.position;
	if (effect.kind == EffectKind::HitSpark && _entityPositions.contains(effect.entityId)) {
		position = _entityPositions.at(effect.entityId);
	}
	switch (effect.kind) {
		case EffectKind::ShotBurst:
			drawAnimation(_window, _assets, sprites::SHOT_SHEET, sprites::SHOT_BURST_FRAMES, position, age, BURST_FRAME_TIME, 2.0F);
			break;
		case EffectKind::HitSpark:
			drawAnimation(_window, _assets, sprites::SHOT_SHEET, sprites::HIT_SPARK_FRAMES, position, age, IMPACT_FRAME_TIME, 2.0F);
			break;
		case EffectKind::MobExplosion:
			drawAnimation(_window, _assets, sprites::EXPLOSION_SHEET, sprites::MOB_EXPLOSION_FRAMES, position, age, MOB_EXPLOSION_FRAME_TIME, 2.0F);
			break;
		case EffectKind::PlayerExplosion:
			drawAnimation(_window, _assets, sprites::EXPLOSION_SHEET, sprites::PLAYER_EXPLOSION_FRAMES, position, age, PLAYER_EXPLOSION_FRAME_TIME, 2.0F);
			break;
	}
}

/// Converts new gameplay notices into sprite animations and draws active frames.
void Sfml::displayEvents(const std::vector<network::WorldEventNotice>& events, const std::optional<network::WorldSnapshot>& snapshot)
{
	if (snapshot && (!_positionsTick || *_positionsTick != snapshot->tick)) {
		_entityPositions.clear();
		for (const network::WorldEntity& entity : snapshot->entities) {
			_entityPositions.insert_or_assign(entity.entityId, sf::Vector2f{entity.x, entity.y});
		}
		_positionsTick = snapshot->tick;
	}

	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::erase_if(_effects, [now](const VisualEffect& effect) { return now - effect.started >= effectDuration(effect.kind); });
	for (const network::WorldEventNotice& notice : events) {
		if (std::holds_alternative<network::EntitySpawnEvent>(notice.data)) {
			const network::EntitySpawnEvent& spawn = std::get<network::EntitySpawnEvent>(notice.data);
			const sf::Vector2f position{spawn.entity.x, spawn.entity.y};
			_entityPositions.insert_or_assign(spawn.entity.entityId, position);
			if (spawn.entity.entityType == network::EntityType::PlayerProjectile) {
				addEffect(spawn.entity.entityId, EffectKind::ShotBurst, position);
			}
		} else if (std::holds_alternative<network::EntityDestroyEvent>(notice.data)) {
			const network::EntityDestroyEvent& destroy = std::get<network::EntityDestroyEvent>(notice.data);
			_entityPositions.erase(destroy.entityId);
			std::erase_if(_effects, [&destroy](const VisualEffect& effect) {
				return effect.entityId == destroy.entityId && effect.kind == EffectKind::HitSpark;
			});
			if (destroy.kind == network::EntityType::Enemy) {
				addEffect(destroy.entityId, EffectKind::MobExplosion, {destroy.x, destroy.y});
			}
		} else if (std::holds_alternative<network::EntityDamageEvent>(notice.data)) {
			const network::EntityDamageEvent& damage = std::get<network::EntityDamageEvent>(notice.data);
			if (_entityPositions.contains(damage.entityId)) {
				addEffect(damage.entityId, EffectKind::HitSpark, _entityPositions.at(damage.entityId));
			}
		} else if (std::holds_alternative<network::PlayerDeathEvent>(notice.data)) {
			const network::PlayerDeathEvent& death = std::get<network::PlayerDeathEvent>(notice.data);
			_entityPositions.erase(death.entityId);
			std::erase_if(_effects, [&death](const VisualEffect& effect) {
				return effect.entityId == death.entityId && effect.kind == EffectKind::HitSpark;
			});
			addEffect(death.entityId, EffectKind::PlayerExplosion, {death.x, death.y});
		}
	}
	const std::chrono::steady_clock::time_point renderedAt = std::chrono::steady_clock::now();
	for (const VisualEffect& effect : _effects) {
		drawEffect(effect, renderedAt);
	}
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
	displayEvents(events, snapshot);
	_window.display();
}

}
