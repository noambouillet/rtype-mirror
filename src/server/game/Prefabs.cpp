/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Entity prefabs: one function per kind of game entity
*/

#include "Prefabs.hpp"

#include "GameConfig.hpp"
#include "server/ecs/GameComponents.hpp"
#include "server/ecs/Systems/ReplicationSystem.hpp"
#include "utils/network/Protocol.hpp"

namespace rtype::prefabs {

/// Builds a box collider centred on the entity's position.
static ecs::Collider centredCollider(float width, float height, std::uint32_t tag, std::uint32_t mask)
{
	return ecs::Collider{width, height, -width / 2.0f, -height / 2.0f, tag, mask};
}

/// Tells whether one more replicated entity still fits in a snapshot; the RFC delays spawns past 64.
bool canSpawnReplicated(ecs::Coordinator& coordinator)
{
	return coordinator.getSystem<ecs::ReplicationSystem>().entities.size() < network::MAX_SNAPSHOT_ENTITIES;
}

/// Creates the ship of `playerId` at its starting slot on the left side.
ecs::Entity createPlayer(ecs::Coordinator& coordinator, std::uint8_t playerId)
{
	const ecs::Entity entity = coordinator.createEntity();
	const float startY = config::PLAYER_START_Y + static_cast<float>(playerId) * config::PLAYER_START_SPACING;

	coordinator.addComponent(entity, ecs::Position{config::PLAYER_START_X, startY});
	coordinator.addComponent(entity, ecs::Velocity{});
	coordinator.addComponent(entity, ecs::Health{config::PLAYER_HEALTH, config::PLAYER_HEALTH});
	coordinator.addComponent(entity, ecs::PlayerControl{playerId, config::PLAYER_SPEED});
	coordinator.addComponent(entity, ecs::Weapon{config::FIRE_COOLDOWN, 0.0f, false});
	coordinator.addComponent(entity, centredCollider(config::PLAYER_WIDTH, config::PLAYER_HEIGHT,
										 ecs::layer::PLAYER, ecs::layer::ENEMY | ecs::layer::ENEMY_PROJECTILE));
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::Player, playerId, playerId});
	return entity;
}

/// Creates a projectile at the nose of a player's ship, flying right until it hits an enemy or leaves the world.
/// `origin` is taken by value: adding components may reallocate the storage a reference would point into.
ecs::Entity createPlayerProjectile(ecs::Coordinator& coordinator, std::uint8_t playerId, ecs::Position origin)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, ecs::Position{origin.x + config::PLAYER_WIDTH / 2.0f, origin.y});
	coordinator.addComponent(entity, ecs::Velocity{config::PROJECTILE_SPEED, 0.0f});
	coordinator.addComponent(entity, ecs::Damage{config::PROJECTILE_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::PROJECTILE_WIDTH, config::PROJECTILE_HEIGHT,
										 ecs::layer::PLAYER_PROJECTILE, ecs::layer::ENEMY));
	coordinator.addComponent(entity, ecs::DestroyOnHit{});
	coordinator.addComponent(entity, ecs::DestroyOffscreen{});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::PlayerProjectile, playerId, 0});
	return entity;
}

/// Creates a Bydo just past the right edge at height `y`, flying left; it hurts players on contact.
ecs::Entity createEnemy(ecs::Coordinator& coordinator, float y)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, ecs::Position{config::WORLD_WIDTH + config::ENEMY_WIDTH / 2.0f, y});
	coordinator.addComponent(entity, ecs::Velocity{-config::ENEMY_SPEED, 0.0f});
	coordinator.addComponent(entity, ecs::Health{config::ENEMY_HEALTH, config::ENEMY_HEALTH});
	coordinator.addComponent(entity, ecs::Damage{config::ENEMY_CONTACT_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::ENEMY_WIDTH, config::ENEMY_HEIGHT,
										 ecs::layer::ENEMY, ecs::layer::PLAYER | ecs::layer::PLAYER_PROJECTILE));
	coordinator.addComponent(entity, ecs::DestroyOffscreen{});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::Enemy, network::NO_OWNER, 0});
	return entity;
}

/// Creates an invisible (not replicated) entity that sends a wave of enemies every WAVE_INTERVAL seconds.
ecs::Entity createWaveSpawner(ecs::Coordinator& coordinator)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, ecs::WaveSpawner{config::WAVE_INTERVAL, config::WAVE_INTERVAL, config::WAVE_MIN_SIZE, config::WAVE_MAX_SIZE});
	return entity;
}

}
