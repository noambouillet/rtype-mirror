/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** DamageSystem
*/

#include "DamageSystem.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (Health + Damage).
Signature DamageSystem::signature(Coordinator& coordinator) {
    Signature sig;

    sig.set(coordinator.getComponentType<Health>());
    sig.set(coordinator.getComponentType<Damage>());
    return sig;
}

/// Applies each entity's Damage to its Health for a frame of dt seconds.
void DamageSystem::update(Coordinator& coordinator) {
    for (const Entity& e : entities) {
        auto& health = coordinator.getComponent<Health>(e);
        const auto& damage = coordinator.getComponent<Damage>(e);

        health.hp = health.hp - damage.amount;
    }
}
} // namespace rtype::ecs
