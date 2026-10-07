/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** RenderSystem
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

#include "Components.hpp"
#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

using TextureMap = std::unordered_map<std::string, sf::Texture>;

class RenderSystem : public System {
public:
	static constexpr float WORLD_WIDTH = 1280.0f;
	static constexpr float WORLD_HEIGHT = 720.0f;

	RenderSystem();
	Signature signature(Coordinator& coordinator);
	void draw(Coordinator& coordinator, sf::RenderWindow& window, const TextureMap& textures);
	sf::View& getWorldView();
	const sf::View& getUIView() const;

private:
	void drawEntity(Coordinator& coordinator, sf::RenderWindow& window, Entity e, const TextureMap& textures);

	sf::View _worldView;
	sf::View _uiView;
};

}  // namespace rtype::ecs
