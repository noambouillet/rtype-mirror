/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** EnemySpawnSystem
*/

#include "EnemySpawnSystem.hpp"

#include "server/ecs/GameComponents.hpp"
#include "server/game/GameConfig.hpp"
#include "server/game/Prefabs.hpp"

namespace rtype::ecs {

/// Seeds the random generator used for wave sizes and enemy heights.
EnemySpawnSystem::EnemySpawnSystem() : _rng(std::random_device{}())
{
}

/// Builds the signature the system needs (WaveSpawner).
Signature EnemySpawnSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<WaveSpawner>());
	return sig;
}

/// Counts each spawner down and sends a wave when it reaches zero.
/// `active` is true when at least one player is connected
void EnemySpawnSystem::update(Coordinator& coordinator, float elapsedTime, bool active)
{
	if (!active)
		return;
	for (const Entity& e : entities) {
		auto& spawner = coordinator.getComponent<WaveSpawner>(e);

		spawner.timer -= elapsedTime;
		if (spawner.timer > 0.0f)
			continue;
		spawner.timer += spawner.interval;
		const int minSize = spawner.minSize;
		const int maxSize = spawner.maxSize;
		spawnWave(coordinator, minSize, maxSize);
	}
}

/// Creates between `minSize` and `maxSize` enemies at random heights on the right edge.
void EnemySpawnSystem::spawnWave(Coordinator& coordinator, int minSize, int maxSize)
{
	std::uniform_int_distribution<int> size(minSize, maxSize);
	std::uniform_real_distribution<float> height(config::ENEMY_HEIGHT, config::WORLD_HEIGHT - config::ENEMY_HEIGHT);
	const int count = size(_rng);

	for (int i = 0; i < count && prefabs::canSpawnReplicated(coordinator); i++) {
		prefabs::createEnemy(coordinator, height(_rng));
	}
}

}
