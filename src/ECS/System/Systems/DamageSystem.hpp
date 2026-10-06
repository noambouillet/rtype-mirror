/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** DamageSystem
*/

#pragma once

#include <vector>

#include "CollisionSystem.hpp"
#include "Components.hpp"
#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class DamageSystem : public System {
public:
	static Signature signature(Coordinator& coordinator);
	void update(Coordinator& coordinator, const std::vector<CollisionPair>& collisions);

private:
	void applyDamage(Coordinator& coordinator, Entity attacker, Entity target);
};

}
