/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** HitSystem
*/

#include "HitSystem.hpp"

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (DestroyOnHit).
Signature HitSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<DestroyOnHit>());
	return sig;
}

/// Marks both sides of each pair, keeping only those that carry `DestroyOnHit`.
void HitSystem::update(Coordinator& coordinator, const std::vector<CollisionPair>& collisions)
{
	for (const auto& [a, b] : collisions) {
		markHit(coordinator, a);
		markHit(coordinator, b);
	}
}

/// Adds `Destroy` to `entity` when this system tracks it and it is not already marked.
void HitSystem::markHit(Coordinator& coordinator, Entity entity)
{
	if (entities.contains(entity) && !coordinator.hasComponent<Destroy>(entity))
		coordinator.addComponent(entity, Destroy{});
}

}
