/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** EntityManager
*/

#include "EntityManager.hpp"

#include <algorithm>

namespace ecs {

/// The capacity is clamped so no valid entity can ever equal the null entity.
EntityManager::EntityManager(std::size_t maxEntities)
    : _maxEntities(
          std::min(maxEntities, static_cast<std::size_t>(Entity::INDEX_MASK))),
      _aliveCount(0) {}

/// Creates an entity, or returns std::nullopt when the capacity is reached.
/// A freed slot is recycled only once more than MIN_FREE_INDICES are waiting.
std::optional<Entity> EntityManager::create() {
  const bool canGrow = _generations.size() < _maxEntities;
  const bool canRecycle = !_freeIndices.empty();
  const bool shouldRecycle = _freeIndices.size() > MIN_FREE_INDICES || !canGrow;
  Entity::Index index = 0;

  if (canRecycle && shouldRecycle) {
    index = _freeIndices.front();
    _freeIndices.pop();
  } else if (canGrow) {
    index = static_cast<Entity::Index>(_generations.size());
    _generations.push_back(0);
    _alive.push_back(false);
  } else {
    return std::nullopt;
  }
  _alive[index] = true;
  _aliveCount++;

  return Entity(index, _generations[index]);
}

/// Destroys an entity. Returns false if the handle is stale or already dead.
/// Bumping the generation invalidates every handle still pointing to the slot.
bool EntityManager::destroy(Entity entity) {
  if (!isAlive(entity)) {
    return false;
  }

  const Entity::Index index = entity.index();

  _alive[index] = false;
  _generations[index]++;
  _freeIndices.push(index);
  _aliveCount--;

  return true;
}

/// An entity is alive if its slot exists, is in use, and generations match.
bool EntityManager::isAlive(Entity entity) const {
  if (entity.isNull() || entity.index() >= _generations.size()) {
    return false;
  }

  return _alive[entity.index()] &&
         _generations[entity.index()] == entity.generation();
}

/// Returns the exact number of currently living entities.
std::size_t EntityManager::aliveCount() const { return _aliveCount; }

/// Returns the maximum number of simultaneously living entities.
std::size_t EntityManager::maxEntities() const { return _maxEntities; }

}  // namespace ecs