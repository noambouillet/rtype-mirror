/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** WeaponSystem
*/

#include "WeaponSystem.hpp"

#include <algorithm>

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"
#include "server/game/Prefabs.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (Weapon + Position): every entity able to shoot.
Signature WeaponSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<Weapon>());
	sig.set(coordinator.getComponentType<Position>());
	return sig;
}

/// Ticks each cooldown and fires for the weapons that are requested to and ready.
void WeaponSystem::update(Coordinator& coordinator, float dt)
{
	for (const Entity& e : entities) {
		auto& weapon = coordinator.getComponent<Weapon>(e);

		weapon.remaining = std::max(0.0f, weapon.remaining - dt);
		if (!weapon.firing || weapon.remaining > 0.0f || !prefabs::canSpawnReplicated(coordinator))
			continue;
		weapon.remaining = weapon.cooldown;
		fire(coordinator, e);
	}
}

/// Spawns the projectile matching the shooter; only player ships carry a weapon for now.
void WeaponSystem::fire(Coordinator& coordinator, Entity shooter)
{
	const Position origin = coordinator.getComponent<Position>(shooter);

	if (coordinator.hasComponent<PlayerControl>(shooter))
		prefabs::createPlayerProjectile(coordinator, coordinator.getComponent<PlayerControl>(shooter).playerId, origin);
}

}
