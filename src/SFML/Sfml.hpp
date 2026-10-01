/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** Sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

constexpr float velocity = 3.0f;

class Sfml {
    public:
        Sfml();
        ~Sfml() = default;
        bool isOpen() const;
        void handleEvent();
        void handleJoystick();
        void handleArrows();
        void handleMove(sf::Vector2f movement);
        void displayWindow();
    private:
        sf::RenderWindow  _window;
        sf::Event _event;
        sf::Texture _playerTexture;
        sf::Sprite _playerSprite;
        unsigned int _joystickId;
};
