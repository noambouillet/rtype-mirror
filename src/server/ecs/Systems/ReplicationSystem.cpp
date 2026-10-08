/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ReplicationSystem
*/

#include "ReplicationSystem.hpp"

#include <algorithm>

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (Position + Replicated): the entities clients can see.
Signature ReplicationSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<Position>());
	sig.set(coordinator.getComponentType<Replicated>());
	return sig;
}

/// Lists what clients must see of every replicated entity (at most 64), health clamped to the u8 range.
std::vector<ReplicatedState> ReplicationSystem::collect(Coordinator& coordinator)
{
	std::vector<ReplicatedState> states;
	states.reserve(std::min(entities.size(), network::MAX_SNAPSHOT_ENTITIES));

	for (const Entity& e : entities) {
		if (states.size() >= network::MAX_SNAPSHOT_ENTITIES)
			break;
		const auto& info = coordinator.getComponent<Replicated>(e);
		const auto& position = coordinator.getComponent<Position>(e);
		int health = 0;
		if (coordinator.hasComponent<Health>(e))
			health = std::clamp(coordinator.getComponent<Health>(e).hp, 0, 255);
		states.push_back({e.id(), info.type, info.owner, info.variant, static_cast<std::uint8_t>(health), position.x, position.y});
	}
	return states;
}

}
