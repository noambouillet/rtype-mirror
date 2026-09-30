/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** IComponentArray
*/

#pragma once
#include "Types.hpp"

class IComponentArray {
    public:
        IComponentArray() = default;
        ~IComponentArray() = default;
        virtual void entityDestroyed(Entity e) = 0;
};
