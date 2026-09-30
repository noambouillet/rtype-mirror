/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** ComponentArray
*/

#pragma once
#include "ComponentArray.hpp"



///Insert a component for the given entity.
template<typename T>
void ComponentArray<T>::insert(Entity e, T comp)
{
    assert(_entityToIndex.find(e) == _entityToIndex.end() && "component already exist");
    _entityToIndex[e] = _size;
    _indexToEntity[_size] = e;
    _data[_size] = comp;
    _size++;
}


///Remove the component of the given entity, keeping the array packed.
template<typename T>
void ComponentArray<T>::remove(Entity e)
{
    assert(_entityToIndex.find(e) != _entityToIndex.end() && "component doesn't exist");
    size_t removed = _entityToIndex[e];
    Entity lastEntity = _indexToEntity[_size - 1];

    _data[removed] = _data[_size -1];
    _entityToIndex[lastEntity] = removed;
    _indexToEntity[removed] = lastEntity;
    _entityToIndex.erase(e);
    _indexToEntity.erase(_size - 1);
    _size--;
}


///Get a reference to the component of the given entity.
template<typename T>
T& ComponentArray<T>::get(Entity e)
{
    assert(_entityToIndex.find(e) != _entityToIndex.end() && "component doesn't exist");
    return _data[_entityToIndex[e]];
}


///Remove the entity's component if it has one.
template<typename T>
void ComponentArray<T>::entityDestroyed(Entity e)
{
    if (_entityToIndex.find(e) != _entityToIndex.end())
        remove(e);
}
