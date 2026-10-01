/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** System
*/

#pragma once

#include <set>

#include "Entity.hpp"

namespace ecs {

class System {
 public:
  virtual ~System() = default;

  std::set<Entity> entities;
};

}  // namespace ecs