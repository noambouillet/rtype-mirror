/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** DamageSystem
*/

#include "DamageSystem.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (Health): the entities that can be damaged.
Signature DamageSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<Health>());
	return sig;
}

/// Applies `Damage` in both directions for every pair in `collisions`.
void DamageSystem::update(Coordinator& coordinator, const std::vector<CollisionPair>& collisions)
{
	for (const auto& [a, b] : collisions) {
		applyDamage(coordinator, a, b);
		applyDamage(coordinator, b, a);
	}
}

/// Removes the `attacker`'s Damage from the `target`'s Health.
/// Marks the target with `Destroy` once its health reaches zero.
void DamageSystem::applyDamage(Coordinator& coordinator, Entity attacker, Entity target)
{
	if (!entities.contains(target) || !coordinator.hasComponent<Damage>(attacker))
		return;

	auto& health = coordinator.getComponent<Health>(target);
	const auto& damage = coordinator.getComponent<Damage>(attacker);

	health.hp -= damage.amount;
	if (health.hp <= 0 && !coordinator.hasComponent<Destroy>(target))
		coordinator.addComponent(target, Destroy{});
}

}
