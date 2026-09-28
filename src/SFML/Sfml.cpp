/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** Sfml
*/

#include "Sfml.hpp"

/// @brief Constructs the Sfml graphics manager, window, and player sprite.
Sfml::Sfml() : _window(sf::VideoMode({1920, 1080}), "rtype"), _joystickId(0)
{
    _window.setFramerateLimit(60);
    if (!_playerTexture.loadFromFile("sprite.png"))
        std::cerr << "Can't load texture" << std::endl;
    _playerSprite.setTexture(_playerTexture);
    _playerSprite.setScale(sf::Vector2f(0.2f, 0.2f));
    _playerSprite.setPosition(100.0f, 200.0f);
}

/// @brief Checks whether the window is currently open.
bool Sfml::isOpen() const
{
    return _window.isOpen();
}

/// @brief Handles input signals coming from the connected joystick.
void Sfml::handleJoystick()
{
    float axisX = sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::X);
    float axisY = sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Y);
    float threshold = 50.0f;

    if (axisX < -threshold)
        handleMove(sf::Vector2(-velocity, 0.0f));
    if (axisX > threshold)
        handleMove(sf::Vector2(velocity, 0.0f));
    if (axisY < -threshold)
        handleMove(sf::Vector2(0.0f, -velocity));
    if (axisY > threshold)
        handleMove(sf::Vector2(0.0f, velocity));
    if (sf::Joystick::isButtonPressed(_joystickId, 0))
        std::cout << "coucou les bg" << std::endl;
    if (sf::Joystick::isButtonPressed(_joystickId, 1))
        std::cout << "nan youki gros" << std::endl;
}

/// @brief Handles keyboard arrow inputs to control movement.
void Sfml::handleArrows()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        handleMove(sf::Vector2f(-velocity, 0.0f));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        handleMove(sf::Vector2f(velocity, 0.0f));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        handleMove(sf::Vector2f(0.0f, -velocity));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        handleMove(sf::Vector2f(0.0f, velocity));
}

/// @brief Moves the player sprite by a given offset vector.
void Sfml::handleMove(sf::Vector2f movement)
{
    _playerSprite.move(movement.x, movement.y);
}

/// @brief Polls window events and routes inputs to joystick or keyboard handlers.
void Sfml::handleEvent()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed || (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape))
            _window.close();
    }
    if (sf::Joystick::isConnected(_joystickId))
        handleJoystick();
    else
        handleArrows();
}

/// @brief Clears, draws entities, and displays the current frame.
void Sfml::displayWindow()
{
    _window.clear(sf::Color::Black);
    _window.setView(_window.getDefaultView());
    _window.draw(_playerSprite);
    _window.display();
}

/// @brief just to test my features
int main(void) {
    Sfml game;

    while (game.isOpen()) {
        game.handleEvent();
        game.displayWindow();
    }
    return 0;
}