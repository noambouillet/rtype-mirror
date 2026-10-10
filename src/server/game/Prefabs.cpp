/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Entity prefabs: one function per kind of game entity
*/

#include "Prefabs.hpp"

#include <cmath>

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
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Block});
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
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::PlayerProjectile, playerId, 0});
	return entity;
}

/// Creates an enemy shot at `origin`, flying at `velocity` (aimed by WeaponSystem) until it hits a player or leaves
/// the world. `origin` is taken by value for the same reason as in createPlayerProjectile.
ecs::Entity createEnemyProjectile(ecs::Coordinator& coordinator, ecs::Position origin, ecs::Velocity velocity)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, origin);
	coordinator.addComponent(entity, velocity);
	coordinator.addComponent(entity, ecs::Damage{config::ENEMY_PROJECTILE_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::ENEMY_PROJECTILE_WIDTH, config::ENEMY_PROJECTILE_HEIGHT,
										 ecs::layer::ENEMY_PROJECTILE, ecs::layer::PLAYER));
	coordinator.addComponent(entity, ecs::DestroyOnHit{});
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::EnemyProjectile, network::NO_OWNER, 0});
	return entity;
}

/// Creates a Scant charge-shot beam at the cannon of a Scant hovering at `origin`, flying straight left until it hits
/// a player or leaves the world. `origin` is taken by value for the same reason as in createPlayerProjectile.
ecs::Entity createScantBeam(ecs::Coordinator& coordinator, ecs::Position origin)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, ecs::Position{origin.x - (config::SCANT_WIDTH + config::SCANT_BEAM_WIDTH) / 2.0f,
										 origin.y + config::SCANT_CANNON_Y});
	coordinator.addComponent(entity, ecs::Velocity{-config::SCANT_BEAM_SPEED, 0.0f});
	coordinator.addComponent(entity, ecs::Damage{config::SCANT_BEAM_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::SCANT_BEAM_WIDTH, config::SCANT_BEAM_HEIGHT,
										 ecs::layer::ENEMY_PROJECTILE, ecs::layer::PLAYER));
	coordinator.addComponent(entity, ecs::DestroyOnHit{});
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::EnemyProjectile, network::NO_OWNER,
										 static_cast<std::uint8_t>(ecs::ShotKind::Beam)});
	return entity;
}

/// Creates a Patapata just past the right edge, weaving around `baseY`; `phase` sets where on its wave it starts,
/// so the members of a group do not rise and fall together. SineMotionSystem makes it weave left.
/// It fires at the players `firstShot` seconds after spawning, then every PATAPATA_FIRE_COOLDOWN seconds.
ecs::Entity createPatapata(ecs::Coordinator& coordinator, float baseY, float phase, float firstShot)
{
	const ecs::Entity entity = coordinator.createEntity();
	const float startY = baseY + config::PATAPATA_AMPLITUDE * std::sin(phase);

	coordinator.addComponent(entity, ecs::Position{config::WORLD_WIDTH + config::PATAPATA_WIDTH / 2.0f, startY});
	coordinator.addComponent(entity, ecs::Velocity{});
	coordinator.addComponent(entity, ecs::SineMotion{config::PATAPATA_SPEED, config::PATAPATA_AMPLITUDE, config::PATAPATA_FREQUENCY, phase, 0.0f});
	coordinator.addComponent(entity, ecs::Weapon{config::PATAPATA_FIRE_COOLDOWN, firstShot, true, ecs::ShotKind::Aimed});
	coordinator.addComponent(entity, ecs::Health{config::PATAPATA_HEALTH, config::PATAPATA_HEALTH});
	coordinator.addComponent(entity, ecs::Damage{config::ENEMY_CONTACT_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::PATAPATA_WIDTH, config::PATAPATA_HEIGHT,
										 ecs::layer::ENEMY, ecs::layer::PLAYER | ecs::layer::PLAYER_PROJECTILE));
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::Enemy, network::NO_OWNER,
										 static_cast<std::uint8_t>(ecs::EnemyKind::Patapata)});
	return entity;
}

/// Creates a Bug just past the right edge at height `y`; PathFollowSystem flies it along the arch.
/// It fires at the players `firstShot` seconds after spawning, then every BUG_FIRE_COOLDOWN seconds.
ecs::Entity createBug(ecs::Coordinator& coordinator, float y, float firstShot)
{
	const ecs::Entity entity = coordinator.createEntity();
	const float startX = config::WORLD_WIDTH + config::BUG_WIDTH / 2.0f;

	coordinator.addComponent(entity, ecs::Position{startX, y});
	coordinator.addComponent(entity, ecs::Velocity{});
	coordinator.addComponent(entity, ecs::PathFollow{ecs::PathId::Arc, 0, config::BUG_SPEED, startX, y});
	coordinator.addComponent(entity, ecs::Weapon{config::BUG_FIRE_COOLDOWN, firstShot, true, ecs::ShotKind::Aimed});
	coordinator.addComponent(entity, ecs::Health{config::BUG_HEALTH, config::BUG_HEALTH});
	coordinator.addComponent(entity, ecs::Damage{config::ENEMY_CONTACT_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::BUG_WIDTH, config::BUG_HEIGHT,
										 ecs::layer::ENEMY, ecs::layer::PLAYER | ecs::layer::PLAYER_PROJECTILE));
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::Enemy, network::NO_OWNER,
										 static_cast<std::uint8_t>(ecs::EnemyKind::Bug)});
	return entity;
}

/// Creates a Bink just past the right edge, standing on the ground and walking left at a constant speed.
ecs::Entity createBink(ecs::Coordinator& coordinator, float firstShot)
{
	const ecs::Entity entity = coordinator.createEntity();
	coordinator.addComponent(entity, ecs::Position{config::WORLD_WIDTH + config::BINK_WIDTH / 2.0f,
										 config::GROUND_Y - config::BINK_HEIGHT / 2.0f});
	coordinator.addComponent(entity, ecs::Velocity{-config::BINK_SPEED, 0.0f});
	coordinator.addComponent(entity, ecs::Weapon{config::BINK_FIRE_COOLDOWN, firstShot, true, ecs::ShotKind::Aimed});
	coordinator.addComponent(entity, ecs::Health{config::BINK_HEALTH, config::BINK_HEALTH});
	coordinator.addComponent(entity, ecs::Damage{config::ENEMY_CONTACT_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::BINK_WIDTH, config::BINK_HEIGHT,
										 ecs::layer::ENEMY, ecs::layer::PLAYER | ecs::layer::PLAYER_PROJECTILE));
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::Enemy, network::NO_OWNER,
										 static_cast<std::uint8_t>(ecs::EnemyKind::Bink)});
	return entity;
}

/// Creates a Scant just past the right edge at height `y`; HoverMotionSystem flies it in, keeps it level with the
/// closest player, then sends it off.
ecs::Entity createScant(ecs::Coordinator& coordinator, float y, float firstShot)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, ecs::Position{config::WORLD_WIDTH + config::SCANT_WIDTH / 2.0f, y});
	coordinator.addComponent(entity, ecs::Velocity{});
	coordinator.addComponent(entity, ecs::HoverMotion{config::SCANT_SPEED, config::SCANT_HOLD_X, config::SCANT_TRACK_SPEED,
										 config::SCANT_STAY_TIME, config::SCANT_CHARGE_TIME, 0.0f});
	coordinator.addComponent(entity, ecs::Weapon{config::SCANT_FIRE_COOLDOWN, firstShot, true, ecs::ShotKind::Beam});
	coordinator.addComponent(entity, ecs::Health{config::SCANT_HEALTH, config::SCANT_HEALTH});
	coordinator.addComponent(entity, ecs::Damage{config::ENEMY_CONTACT_DAMAGE});
	coordinator.addComponent(entity, centredCollider(config::SCANT_WIDTH, config::SCANT_HEIGHT,
										 ecs::layer::ENEMY, ecs::layer::PLAYER | ecs::layer::PLAYER_PROJECTILE));
	coordinator.addComponent(entity, ecs::Offscreen{ecs::OffscreenAction::Destroy});
	coordinator.addComponent(entity, ecs::Replicated{network::EntityType::Enemy, network::NO_OWNER,
										 static_cast<std::uint8_t>(ecs::EnemyKind::Scant)});
	return entity;
}

/// Creates an invisible (not replicated) entity that sends `count` enemies of `kind` around height `y`, the first one
/// next tick: Bugs all start at `y` so they trace one shared path, Patapata are scattered so each weaves on its own.
/// Binks ignore `y`: they always walk on the ground.
ecs::Entity createFormation(ecs::Coordinator& coordinator, ecs::EnemyKind kind, float y, int count)
{
	const ecs::Entity entity = coordinator.createEntity();
	const float delay = kind == ecs::EnemyKind::Bug ? config::BUG_DELAY : config::PATAPATA_DELAY;
	const float spread = kind == ecs::EnemyKind::Patapata ? config::PATAPATA_SPREAD : 0.0f;

	coordinator.addComponent(entity, ecs::FormationSpawner{kind, count, delay, 0.0f, y, spread});
	return entity;
}

/// Creates an invisible (not replicated) entity that sends a wave of enemies every WAVE_INTERVAL seconds.
ecs::Entity createWaveSpawner(ecs::Coordinator& coordinator)
{
	const ecs::Entity entity = coordinator.createEntity();

	coordinator.addComponent(entity, ecs::WaveSpawner{config::WAVE_INTERVAL, config::WAVE_INTERVAL});
	return entity;
}

}
