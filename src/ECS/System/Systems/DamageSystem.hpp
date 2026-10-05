/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** DamageSystem
*/

#pragma once

#include "Components.hpp"
#include "Coordinator.hpp"
#include "System.hpp"

namespace rtype::ecs {

class DamageSystem : public System {
 public:
  static Signature signature(Coordinator& coordinator);
  void update(Coordinator& coordinator);
};

}  // namespace rtype::ecs
