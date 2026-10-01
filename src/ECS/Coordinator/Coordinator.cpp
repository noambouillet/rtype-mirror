/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Coordinator
*/

#include "Coordinator.hpp"
#include <stdexcept>

namespace rtype::ecs {
Coordinator::Coordinator(std::size_t maxEntities)
    : _entities(maxEntities), _components(_entities.maxEntities()) {}

///Create a new entity. Throws if the maximum number of entities is reached.
Entity Coordinator::createEntity()
{
    std::optional<Entity> newEntity = _entities.create();

    if (!newEntity)
        throw std::runtime_error("Coordinator: max entities reached");
    return *newEntity;
}

///Destroy an entity and remove it from every component array and system.
///Does nothing if the handle is stale or already dead.
void Coordinator::destroyEntity(Entity e)
{
    if (!_entities.destroy(e))
        return;
    _components.entityDestroyed(e);
    _systems.entityDestroyed(e);
}

///Tell whether the given entity handle still refers to a living entity.
bool Coordinator::isAlive(Entity e) const
{
    return _entities.isAlive(e);
}
} // namespace rtype::ecs
