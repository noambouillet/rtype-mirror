/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** PathFollowSystem
*/

#pragma once

#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class PathFollowSystem : public System {
public:
	Signature signature(Coordinator& coordinator);
	void update(Coordinator& coordinator, float elapsedTime);
};

}
