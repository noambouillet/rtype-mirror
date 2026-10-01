/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** System
*/

#pragma once
#include "Coordinator.hpp"

namespace rtype::ecs {
///Register a new system of type T and return a reference to it.
template<typename T>
T& Coordinator::registerSystem()
{
    return _systems.registerSystem<T>();
}

///Set the component signature an entity needs to be handled by the system T.
template<typename T>
void Coordinator::setSystemSignature(Signature s)
{
    _systems.setSignature<T>(s);
}

///Get a reference to the already registered system T.
template<typename T>
T& Coordinator::getSystem()
{
    return _systems.getSystem<T>();
}
} // namespace rtype::ecs
