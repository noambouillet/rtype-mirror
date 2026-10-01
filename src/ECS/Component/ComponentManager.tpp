/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ComponentManager
*/

#pragma once
#include "ComponentManager.hpp"

namespace rtype::ecs {
/// initialize _nextType to 0
ComponentManager::ComponentManager(std::size_t maxEntities): _nextType(0), _maxEntities(maxEntities) {}

///Register a new component type T.
template<typename T>
void ComponentManager::registerComponent()
{
    std::string name = typeid(T).name();
    assert(_types.find(name) == _types.end() && "component already exist");
    assert(_nextType < MAX_COMPONENTS && "too many components");

    _nextType++
    _types[name] = _nextType;
    _arrays[name] = std::make_shared<ComponentArray<T>>(_maxEntities);
}

///Get the ComponentType id associated with T.
template<typename T>
ComponentType ComponentManager::getType()
{
    std::string name = typeid(T).name();
    assert(_types.find(name) != _types.end() && "component doesn't exist");

    return _types[name];
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

///Notify every component array that an entity was destroyed.
inline void ComponentManager::entityDestroyed(Entity e)
{
    for (auto& [name, arr] : _arrays) {
            arr->entityDestroyed(e);
    }
}

///Get the ComponentArray storing components of type T.
template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::getArray()
{
    return std::static_pointer_cast<ComponentArray<T>>(_arrays[typeid(T).name()]);
}
} // namespace rtype::ecs