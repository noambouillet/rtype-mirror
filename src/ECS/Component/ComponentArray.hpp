/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ComponentArray
*/

#pragma once
#include <IComponentArray.hpp>
#include <array>
#include <unordered_map>
#include <cassert>

namespace ECS {
template<typename T>
class ComponentArray: public IComponentArray {
    public:
        ComponentArray();
        ~ComponentArray() = default;
        void insert(Entity e, T comp);
        void remove(Entity e);
        T& get(Entity e);
        void entityDestroyed(Entity e) override;
    private:
        std::array<T, MAX_ENTITIES> _data;
        std::unordered_map<Entity, size_t> _entityToIndex;
        std::unordered_map<size_t, Entity> _indexToEntity;
        size_t _size;
};

#include "ComponentArray.tpp"
} // namespace ECS