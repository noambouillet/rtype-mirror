/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** WeaponSystem
*/

#include "WeaponSystem.hpp"

#include <algorithm>
#include <cmath>
#include <optional>

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"
#include "server/ecs/Systems/PlayerControlSystem.hpp"
#include "server/game/GameConfig.hpp"
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
/// A weapon that holds its fire stays ready, so it shoots as soon as it gets a target.
void WeaponSystem::update(Coordinator& coordinator, float elapsedTime)
{
	std::vector<Entity> spawned;
	for (const Entity& e : entities) {
		auto& weapon = coordinator.getComponent<Weapon>(e);

		weapon.remaining = std::max(0.0F, weapon.remaining - elapsedTime);
		if (!weapon.firing || weapon.remaining > 0.0F || !prefabs::canSpawnReplicated(coordinator))
			continue;
		if (fire(coordinator, e))
			weapon.remaining = weapon.cooldown;
	}
	return spawned;
}

/// Spawns the projectile matching the shooter: player ships shoot straight ahead, enemies fire their ShotKind once onscreen.
bool WeaponSystem::fire(Coordinator& coordinator, Entity shooter)
{
	const Position origin = coordinator.getComponent<Position>(shooter);

	if (coordinator.hasComponent<PlayerControl>(shooter)) {
		prefabs::createPlayerProjectile(coordinator, coordinator.getComponent<PlayerControl>(shooter).playerId, origin);
		return true;
	}
	if (origin.x > config::WORLD_WIDTH)
		return false;
	if (coordinator.getComponent<Weapon>(shooter).shot == network::ShotKind::Beam) {
		prefabs::createScantBeam(coordinator, origin);
		return true;
	}
	return fireAtPlayer(coordinator, origin);
}

/// Fires a shot from `origin` toward where the closest player is now; the shot then flies straight on.
bool WeaponSystem::fireAtPlayer(Coordinator& coordinator, Position origin)
{
	std::optional<Velocity> aim;
	float closest = 0.0F;

	for (const Entity& player : coordinator.getSystem<PlayerControlSystem>().entities) {
		const auto& target = coordinator.getComponent<Position>(player);
		const float dx = target.x - origin.x;
		const float dy = target.y - origin.y;
		const float distance = std::sqrt(dx * dx + dy * dy);

		if (dx >= 0.0F || distance < config::ENEMY_MIN_FIRE_DISTANCE || (aim && distance >= closest))
			continue;
		closest = distance;
		aim = Velocity{config::ENEMY_PROJECTILE_SPEED * dx / distance, config::ENEMY_PROJECTILE_SPEED * dy / distance};
	}
	if (!aim)
		return false;
	prefabs::createEnemyProjectile(coordinator, origin, *aim);
	return true;
}

}
