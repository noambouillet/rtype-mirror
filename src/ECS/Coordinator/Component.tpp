/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Component
*/

#pragma once
#include <cassert>
#include <utility>
#include "Coordinator.hpp"

namespace rtype::ecs {
///Register a new component type T so it can be added to entities.
template<typename T>
void Coordinator::registerComponent()
{
    _components.registerComponent<T>();
}

///Add a component of type T to the entity, update its signature and notify the systems.
template<typename T>
void Coordinator::addComponent(Entity e, T comp)
{
    assert(_entities.isAlive(e) && "entity is not alive");
    _components.add<T>(e, std::move(comp));
    Signature sig = _entities.getSignature(e);
    sig.set(_components.getType<T>(), true);
    _entities.setSignature(e, sig);
    _systems.entitySignatureChanged(e, sig);
}

///Remove the component of type T from the entity, update its signature and notify the systems.
template<typename T>
void Coordinator::removeComponent(Entity e)
{
    assert(_entities.isAlive(e) && "entity is not alive");
    _components.remove<T>(e);
    Signature sig = _entities.getSignature(e);
    sig.set(_components.getType<T>(), false);
    _entities.setSignature(e, sig);
    _systems.entitySignatureChanged(e, sig);
}

///Get a reference to the component of type T of the given entity.
template<typename T>
T& Coordinator::getComponent(Entity e)
{
    return _components.get<T>(e);
}

///Get the ComponentType id of T, used to build system signatures.
template<typename T>
ComponentType Coordinator::getComponentType()
{
    return _components.getType<T>();
}
} // namespace rtype::ecs
