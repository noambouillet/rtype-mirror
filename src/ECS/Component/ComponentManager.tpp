/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ComponentManager
*/

#pragma once
#include "ComponentManager.hpp"

namespace rtype::ecs {
///Register a new component type T.
template<typename T>
void ComponentManager::registerComponent()
{
    ComponentType id = std::type_index(typeid(T));
    assert(id < MAX_COMPONENTS && "too many components");
    assert(!_arrays[id] && "component already exist");

    _arrays[id] = std::make_shared<ComponentArray<T>>(_maxEntities);
}

///Get the ComponentType id associated with T.
template<typename T>
ComponentType ComponentManager::getType()
{
    ComponentType id = std::type_index(typeid(T));
    assert(id < MAX_COMPONENTS && _arrays[id] && "component doesn't exist");

    return id;
}

///Add a component of type T to the given entity.
template<typename T>
void ComponentManager::add(Entity e, T comp)
{
    getArray<T>()->insert(e, comp);
}

///Remove the component of type T from the given entity.
template<typename T>
void ComponentManager::remove(Entity e)
{
    getArray<T>()->remove(e);
}

///Get a reference to the component of type T of the given entity.
template<typename T>
T& ComponentManager::get(Entity e)
{
    return getArray<T>()->get(e);
}

///Get the ComponentArray storing components of type T.
template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::getArray()
{
    ComponentType id = getType<T>();
    return std::static_pointer_cast<ComponentArray<T>>(_arrays[id]);
}
} // namespace rtype::ecs