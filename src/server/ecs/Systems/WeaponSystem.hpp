/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** WeaponSystem
*/

#pragma once

#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class WeaponSystem : public System {
public:
	Signature signature(Coordinator& coordinator);
	void update(Coordinator& coordinator, float dt);

private:
	void fire(Coordinator& coordinator, Entity shooter);
};

}
