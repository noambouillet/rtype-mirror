/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** FormationSystem
*/

#pragma once

#include <random>

#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class FormationSystem : public System {
public:
	FormationSystem();
	Signature signature(Coordinator& coordinator);
	void update(Coordinator& coordinator, float elapsedTime);

private:
	std::mt19937 _rng;
};

}
