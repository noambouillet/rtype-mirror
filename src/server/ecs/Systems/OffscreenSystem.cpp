/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** OffscreenSystem
*/

#include "OffscreenSystem.hpp"

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (Position + DestroyOffscreen).
Signature OffscreenSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<Position>());
	sig.set(coordinator.getComponentType<DestroyOffscreen>());
	return sig;
}

/// Adds `Destroy` to every tracked entity whose position is beyond the world plus `margin`.
void OffscreenSystem::update(Coordinator& coordinator, float worldWidth, float worldHeight, float margin)
{
	for (const Entity& e : entities) {
		const auto& pos = coordinator.getComponent<Position>(e);
		const bool outside = pos.x < -margin || pos.x > worldWidth + margin || pos.y < -margin || pos.y > worldHeight + margin;

		if (outside && !coordinator.hasComponent<Destroy>(e))
			coordinator.addComponent(e, Destroy{});
	}
}

}
