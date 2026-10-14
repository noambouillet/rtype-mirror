/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Client view world
*/

#include "ClientWorld.hpp"

#include <optional>
#include <vector>

#include "Components.hpp"
#include "RenderSystem.hpp"
#include "client/view/ClientComponents.hpp"
#include "client/view/ClientNetworkSystem.hpp"
#include "client/view/EffectSystem.hpp"

namespace rtype::client {

ClientWorld::ClientWorld()
{
	_coordinator.registerComponent<ecs::Position>();
	_coordinator.registerComponent<ecs::Sprite>();
	_coordinator.registerComponent<ecs::Animation>();
	_coordinator.registerComponent<NetworkIdentity>();
	_coordinator.registerComponent<Effect>();
	auto& network = _coordinator.registerSystem<ClientNetworkSystem>();
	_coordinator.setSystemSignature<ClientNetworkSystem>(network.signature(_coordinator));
	auto& effects = _coordinator.registerSystem<EffectSystem>();
	_coordinator.setSystemSignature<EffectSystem>(effects.signature(_coordinator));
	auto& render = _coordinator.registerSystem<ecs::RenderSystem>();
	_coordinator.setSystemSignature<ecs::RenderSystem>(render.signature(_coordinator));
	render.setBackground("background.png", 40.0F);
}

/// Updates the network copy and temporary presentation state for one client frame.
void ClientWorld::update(network::GameClient& client, std::uint8_t buttons, float deltaTime)
{
	auto& network = _coordinator.getSystem<ClientNetworkSystem>();
	auto& effects = _coordinator.getSystem<EffectSystem>();
	const std::vector<ViewEvent> events = network.update(_coordinator, client, buttons);
	const std::optional<std::uint8_t> playerId = client.status() == network::ClientStatus::Connected ? std::optional<std::uint8_t>(client.playerId()) : std::nullopt;
	effects.consume(_coordinator, network, events, playerId);
	effects.predictLocalFire(_coordinator, network, buttons, playerId);
	effects.update(_coordinator, network, deltaTime);
}

void ClientWorld::render(render::IGraphicsBackend& backend, float deltaTime)
{
	_coordinator.getSystem<ecs::RenderSystem>().render(_coordinator, backend, deltaTime);
}

}
