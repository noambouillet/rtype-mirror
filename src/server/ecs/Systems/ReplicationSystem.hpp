/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ReplicationSystem
*/

#pragma once

#include <cstdint>
#include <vector>

#include "Coordinator.hpp"
#include "System.hpp"
#include "utils/network/Protocol.hpp"

namespace rtype::ecs {

struct ReplicatedState {
	std::uint32_t id;
	network::EntityType type;
	std::uint8_t owner;
	std::uint8_t variant;
	std::uint8_t health;
	float x;
	float y;
};

class ReplicationSystem : public System {
public:
	Signature signature(Coordinator& coordinator);
	std::vector<ReplicatedState> collect(Coordinator& coordinator);
};

}
