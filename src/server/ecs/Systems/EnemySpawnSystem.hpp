/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** EnemySpawnSystem
*/

#pragma once

#include <random>

#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class EnemySpawnSystem : public System {
public:
	EnemySpawnSystem();
	Signature signature(Coordinator& coordinator);
	void update(Coordinator& coordinator, float dt, bool active);

private:
	void spawnWave(Coordinator& coordinator, int minSize, int maxSize);

	std::mt19937 _rng;
};

}
