#include <cmath>
#include "Warrior.hpp"

Warrior::Warrior(float radius, sf::Vector2f position)
    : targetPosition(position), isMoving(false), speed(100.0f), ammo(100) // Começa com 100 balas
{
    shape.setRadius(radius);
    shape.setFillColor(sf::Color(173, 216, 230));
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setPointCount(100);
    shape.setPosition(position);
}

int Warrior::getAmmo() const
{
    return ammo;
}

void Warrior::useAmmo(int amount)
{
    if (ammo >= amount)
        ammo -= amount;
}

void Warrior::addAmmo(int amount)
{
    if (ammo + amount >= 100)
    {
        ammo = 100;
    }
    else
    {
        ammo += amount;
    }
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

sf::Vector2f Warrior::getPosition() const
{
    return shape.getPosition();
}

sf::FloatRect Warrior::getBounds() const
{
    return shape.getGlobalBounds();
}
