/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** WeaponSystem
*/

#pragma once

#include "Components.hpp"
#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class WeaponSystem : public System {
public:
	Signature signature(Coordinator& coordinator);
	void update(Coordinator& coordinator, float elapsedTime);

private:
	bool fire(Coordinator& coordinator, Entity shooter);
	bool fireAtPlayer(Coordinator& coordinator, Position origin);
};

}
