/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** EntityManager
*/

#pragma once

#include <cstddef>
#include <optional>
#include <queue>
#include <vector>

#include "Entity.hpp"

namespace ecs {

class EntityManager {
 public:
  static constexpr std::size_t DEFAULT_MAX_ENTITIES = 8192;
  static constexpr std::size_t MIN_FREE_INDICES = 1024;

  explicit EntityManager(std::size_t maxEntities = DEFAULT_MAX_ENTITIES);
  ~EntityManager() = default;

  [[nodiscard]] std::optional<Entity> create();
  bool destroy(Entity entity);
  [[nodiscard]] bool isAlive(Entity entity) const;
  [[nodiscard]] std::size_t aliveCount() const;
  [[nodiscard]] std::size_t maxEntities() const;

 private:
  std::size_t _maxEntities;
  std::size_t _aliveCount;
  std::vector<Entity::Generation> _generations;
  std::vector<bool> _alive;
  std::queue<Entity::Index> _freeIndices;
};

}  // namespace ecs