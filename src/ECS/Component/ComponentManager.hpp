/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ComponentManager
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include "ComponentArray.hpp"

namespace rtype::ecs {
class ComponentManager {
    public:
        explicit ComponentManager(std::size_t maxEntities);
        ~ComponentManager() = default;
        template<typename T>
        void registerComponent();
        template<typename T>
        ComponentType getType();
        template<typename T>
        void add(Entity e, T comp);
        template<typename T>
        void remove(Entity e);
        template<typename T>
        T& get(Entity e);
        void entityDestroyed(Entity e);
    private:
        std::unordered_map<std::string, ComponentType> _types;
        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _arrays;
        ComponentType _nextType;
        std::size_t _maxEntities;
        template<typename T>
        std::shared_ptr<ComponentArray<T>> getArray();
};
}// namespace rtype::ecs
#include "ComponentManager.tpp"