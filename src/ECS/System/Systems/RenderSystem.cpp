/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"

namespace rtype::ecs {

RenderSystem::RenderSystem()
	: _worldView(sf::FloatRect({0.0f, 0.0f}, {WORLD_WIDTH, WORLD_HEIGHT})),
	  _uiView(sf::FloatRect({0.0f, 0.0f}, {WORLD_WIDTH, WORLD_HEIGHT}))
{
}

/// Builds the signature the system needs (Position + Sprite).
Signature RenderSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<Position>());
	sig.set(coordinator.getComponentType<Sprite>());
	return sig;
}

/// Draws world sprites first, then UI sprites on top, in 1280x720 logical units.
void RenderSystem::draw(Coordinator& coordinator, sf::RenderWindow& window, const TextureMap& textures)
{
	window.setView(_worldView);
	for (const Entity& e : entities) {
		if (!coordinator.getComponent<Sprite>(e).isUI) {
			drawEntity(coordinator, window, e, textures);
		}
	}
	window.setView(_uiView);
	for (const Entity& e : entities) {
		if (coordinator.getComponent<Sprite>(e).isUI) {
			drawEntity(coordinator, window, e, textures);
		}
	}
}

/// Draws one entity centered on its position.
void RenderSystem::drawEntity(Coordinator& coordinator, sf::RenderWindow& window, Entity e, const TextureMap& textures)
{
	const auto& pos = coordinator.getComponent<Position>(e);
	const auto& spr = coordinator.getComponent<Sprite>(e);
	const auto texture = textures.find(spr.textureName);

	if (texture == textures.end()) {
		return;
	}
	sf::Sprite sprite(texture->second);
	const sf::Vector2f size(texture->second.getSize());

	sprite.setOrigin(size / 2.0f);
	sprite.setPosition({pos.x, pos.y});
	sprite.setScale({spr.scale, spr.scale});
	sprite.setRotation(sf::degrees(spr.rotation));
	window.draw(sprite);
}

sf::View& RenderSystem::getWorldView()
{
	return _worldView;
}

const sf::View& RenderSystem::getUIView() const
{
	return _uiView;
}

}  // namespace rtype::ecs
