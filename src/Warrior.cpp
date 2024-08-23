#include <cmath>
#include "Warrior.hpp"

Warrior::Warrior(float radius, sf::Vector2f position)
    : targetPosition(position), isMoving(false), speed(200.0f) // Definimos a velicidade do guerreiro
{
    shape.setRadius(radius);                      // Define o raio da forma
    shape.setFillColor(sf::Color(173, 216, 230)); // Define a cor da forma
    shape.setOutlineThickness(2.0f);              // Define a espessura do contorno
    shape.setOutlineColor(sf::Color::Black);      // Define a cor do contorno
    shape.setPointCount(100);                     // Define um número alto de pontos
    shape.setPosition(position);
}

void Warrior::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

void Warrior::setPosition(sf::Vector2f position)
{
    shape.setPosition(position);
}

void Warrior::moveTo(sf::Vector2f newPosition)
{
    targetPosition = newPosition;
    isMoving = true;
}

void Warrior::update(float deltaTime)
{
    if (isMoving)
    {
        sf::Vector2f currentPosition = shape.getPosition();
        sf::Vector2f direction = targetPosition - currentPosition;
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > speed * deltaTime)
        {
            direction /= distance;
            shape.move(direction * speed * deltaTime);
        }
        else
        {
            shape.setPosition(targetPosition);
            isMoving = false;
        }
    }
}