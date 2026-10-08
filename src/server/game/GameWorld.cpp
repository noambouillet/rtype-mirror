/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Server game world: owns the ECS and runs one gameplay tick
*/

#include "GameWorld.hpp"

#include "CollisionSystem.hpp"
#include "Components.hpp"
#include "DamageSystem.hpp"
#include "DestroySystem.hpp"
#include "MovementSystem.hpp"
#include "server/ecs/GameComponents.hpp"
#include "server/ecs/Systems/EnemySpawnSystem.hpp"
#include "server/ecs/Systems/HitSystem.hpp"
#include "server/ecs/Systems/OffscreenSystem.hpp"
#include "server/ecs/Systems/PlayerControlSystem.hpp"
#include "server/ecs/Systems/WeaponSystem.hpp"
#include "server/game/Prefabs.hpp"

namespace rtype {

/// Registers T and gives it the signature its own `signature()` describes.
template <typename T>
static void registerSystem(ecs::Coordinator& coordinator)
{
	auto& system = coordinator.registerSystem<T>();
	coordinator.setSystemSignature<T>(system.signature(coordinator));
}

/// Creates the world: components, then systems, then the entities that exist from the start.
GameWorld::GameWorld() : _coordinator{}, _playerEntities{}
{
	registerComponents();
	registerSystems();
	prefabs::createWaveSpawner(_coordinator);
}

/// Registers every component type; systems need them to build their signatures.
void GameWorld::registerComponents()
{
	_coordinator.registerComponent<ecs::Position>();
	_coordinator.registerComponent<ecs::Velocity>();
	_coordinator.registerComponent<ecs::Health>();
	_coordinator.registerComponent<ecs::Damage>();
	_coordinator.registerComponent<ecs::Sprite>();
	_coordinator.registerComponent<ecs::Destroy>();
	_coordinator.registerComponent<ecs::Collider>();
	_coordinator.registerComponent<ecs::Replicated>();
	_coordinator.registerComponent<ecs::PlayerControl>();
	_coordinator.registerComponent<ecs::Weapon>();
	_coordinator.registerComponent<ecs::WaveSpawner>();
	_coordinator.registerComponent<ecs::DestroyOffscreen>();
	_coordinator.registerComponent<ecs::DestroyOnHit>();
}

/// Registers every system; it must happen before any entity exists so each system sees them all.
void GameWorld::registerSystems()
{
	registerSystem<ecs::PlayerControlSystem>(_coordinator);
	registerSystem<ecs::WeaponSystem>(_coordinator);
	registerSystem<ecs::EnemySpawnSystem>(_coordinator);
	registerSystem<ecs::MovementSystem>(_coordinator);
	registerSystem<ecs::CollisionSystem>(_coordinator);
	registerSystem<ecs::DamageSystem>(_coordinator);
	registerSystem<ecs::HitSystem>(_coordinator);
	registerSystem<ecs::OffscreenSystem>(_coordinator);
	registerSystem<ecs::DestroySystem>(_coordinator);
	registerSystem<ecs::ReplicationSystem>(_coordinator);
}

/// Advances the world by one fixed tick: players join or leave, then every system runs.
void GameWorld::update(const Input& input, float deltaTime)
{
	syncPlayerEntities(input);
	runSystems(input, deltaTime);
}

/// Runs the systems in dependency order; DestroySystem stays last so every system sees this tick's entities.
void GameWorld::runSystems(const Input& input, float deltaTime)
{
	_coordinator.getSystem<ecs::PlayerControlSystem>().update(_coordinator, input.buttons);
	_coordinator.getSystem<ecs::WeaponSystem>().update(_coordinator, deltaTime);
	_coordinator.getSystem<ecs::EnemySpawnSystem>().update(_coordinator, deltaTime, input.connectedMask != 0);
	_coordinator.getSystem<ecs::MovementSystem>().update(_coordinator, deltaTime);
	auto& collision = _coordinator.getSystem<ecs::CollisionSystem>();
	collision.update(_coordinator);
	_coordinator.getSystem<ecs::DamageSystem>().update(_coordinator, collision.collisions());
	_coordinator.getSystem<ecs::HitSystem>().update(_coordinator, collision.collisions());
	_coordinator.getSystem<ecs::OffscreenSystem>().update(_coordinator, config::WORLD_WIDTH, config::WORLD_HEIGHT, config::OFFSCREEN_MARGIN);
	_coordinator.getSystem<ecs::DestroySystem>().update(_coordinator);
}

// Spawns a ship for each newly connected player and removes the ship of each player who left.
void GameWorld::syncPlayerEntities(const Input& input)
{
	for (std::size_t playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		const std::uint8_t playerMask = static_cast<std::uint8_t>(1 << playerId);
		const bool connected = (input.connectedMask & playerMask) != 0;

		if (connected && !_playerEntities[playerId]) {
			_playerEntities[playerId] = prefabs::createPlayer(_coordinator, static_cast<std::uint8_t>(playerId));
		} else if (!connected && _playerEntities[playerId]) {
			_coordinator.destroyEntity(*_playerEntities[playerId]);
			_playerEntities[playerId].reset();
		}
	}
}

/// Returns the bit field of players whose ship is still alive.
std::uint8_t GameWorld::aliveMask() const
{
	std::uint8_t mask = 0;
	for (std::size_t playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		if (_playerEntities[playerId] && _coordinator.isAlive(*_playerEntities[playerId])) {
			mask |= static_cast<std::uint8_t>(1 << playerId);
		}
	}
	return mask;
}

/// Lists what clients must see this tick; GameServer encodes it into a snapshot.
std::vector<GameWorld::EntityState> GameWorld::replicatedEntities()
{
	return _coordinator.getSystem<ecs::ReplicationSystem>().collect(_coordinator);
}

}
