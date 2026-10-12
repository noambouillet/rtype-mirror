/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** WeaponSystem
*/

#pragma once

#include <optional>
#include <vector>

#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class WeaponSystem : public System {
public:
	Signature signature(Coordinator& coordinator);
	std::vector<Entity> update(Coordinator& coordinator, float dt);

private:
	std::optional<Entity> fire(Coordinator& coordinator, Entity shooter);
};

}
