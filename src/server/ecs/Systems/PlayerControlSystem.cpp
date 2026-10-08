/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** PlayerControlSystem
*/

#include "PlayerControlSystem.hpp"

#include <cmath>

#include "Components.hpp"
#include "server/ecs/GameComponents.hpp"
#include "utils/network/Protocol.hpp"

namespace rtype::ecs {

/// Builds the signature the system needs (PlayerControl + Velocity + Weapon): the ships driven by a client.
Signature PlayerControlSystem::signature(Coordinator& coordinator)
{
	Signature sig;

	sig.set(coordinator.getComponentType<PlayerControl>());
	sig.set(coordinator.getComponentType<Velocity>());
	sig.set(coordinator.getComponentType<Weapon>());
	return sig;
}

/// Turns each player's buttons into a velocity (diagonals normalised) and a fire request for WeaponSystem.
void PlayerControlSystem::update(Coordinator& coordinator, const Buttons& buttons)
{
	for (const Entity& e : entities) {
		const auto& control = coordinator.getComponent<PlayerControl>(e);
		const std::uint8_t pressed = control.playerId < buttons.size() ? buttons[control.playerId] : 0;
		const float horizontal = static_cast<float>((pressed & network::BUTTON_RIGHT) != 0) - static_cast<float>((pressed & network::BUTTON_LEFT) != 0);
		const float vertical = static_cast<float>((pressed & network::BUTTON_DOWN) != 0) - static_cast<float>((pressed & network::BUTTON_UP) != 0);
		const float length = std::sqrt(horizontal * horizontal + vertical * vertical);

		auto& velocity = coordinator.getComponent<Velocity>(e);
		if (length == 0.0f) {
			velocity = {0.0f, 0.0f};
		} else {
			velocity = {control.speed * horizontal / length, control.speed * vertical / length};
		}
		coordinator.getComponent<Weapon>(e).firing = (pressed & network::BUTTON_FIRE) != 0;
	}
}

}
