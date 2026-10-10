/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** PathFollowSystem
*/

#include "PathFollowSystem.hpp"

#include <cmath>
#include <numbers>
#include <vector>

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"
#include "server/game/GameConfig.hpp"

namespace rtype::ecs {

static constexpr int ARC_SEGMENTS = 24;

/// Builds the Bug arch relative to the spawn point: `y = -BUG_ARC_HEIGHT * sin(BUG_ARC_TURNS * π t)` across the whole world.
static std::vector<Position> buildArc()
{
	const float span = config::WORLD_WIDTH + config::BUG_WIDTH + 2.0f * config::OFFSCREEN_MARGIN;
	std::vector<Position> points;

	for (int i = 1; i <= ARC_SEGMENTS; i++) {
		const float t = static_cast<float>(i) / static_cast<float>(ARC_SEGMENTS);
		points.push_back({-t * span, -config::BUG_ARC_HEIGHT * std::sin(config::BUG_ARC_TURNS * std::numbers::pi_v<float> * t)});
	}
	return points;
}

/// Returns the waypoints of `path`, relative to the spawn point; they are built once, on first use.
static const std::vector<Position>& waypoints(PathId path)
{
	static const std::vector<Position> ARC = buildArc();

	switch (path) {
		case PathId::Arc:
			return ARC;
		default:
			return ARC;
	}
}

/// Builds the signature the system needs (PathFollow + Position + Velocity): the enemies tracing a path.
Signature PathFollowSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<PathFollow>());
	sig.set(coordinator.getComponentType<Position>());
	sig.set(coordinator.getComponentType<Velocity>());
	return sig;
}

/// Points each velocity at the next waypoint, skipping the ones reached within this tick.
/// Past the last waypoint the velocity is kept, so the enemy flies on until it leaves the world.
void PathFollowSystem::update(Coordinator& coordinator, float elapsedTime)
{
	for (const Entity& e : entities) {
		auto& follow = coordinator.getComponent<PathFollow>(e);
		const auto& position = coordinator.getComponent<Position>(e);
		const auto& points = waypoints(follow.path);
		const float step = follow.speed * elapsedTime;

		while (follow.next < points.size()) {
			const float dx = follow.originX + points[follow.next].x - position.x;
			const float dy = follow.originY + points[follow.next].y - position.y;
			const float distance = std::sqrt(dx * dx + dy * dy);

			if (distance > step) {
				coordinator.getComponent<Velocity>(e) = {follow.speed * dx / distance, follow.speed * dy / distance};
				break;
			}
			follow.next++;
		}
	}
}

}
