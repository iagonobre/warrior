#include "Projectile.hpp"
#include <cmath>

Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition, sf::Color color)
    : speed(300.0f)
{
    shape.setRadius(5.0f);
    shape.setFillColor(color);
    shape.setPosition(startPosition);
    calculateDirection(startPosition, targetPosition);
}

void Projectile::calculateDirection(sf::Vector2f startPosition, sf::Vector2f targetPosition)
{
    sf::Vector2f delta = targetPosition - startPosition;
    float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
    direction = delta / distance; // Normaliza a direção
}

void Projectile::update(float deltaTime)
{
    shape.move(direction * speed * deltaTime);
}

void Projectile::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

bool Projectile::isOutOfBounds(const sf::RenderWindow &window) const
{
    sf::Vector2f position = shape.getPosition();
    sf::Vector2u windowSize = window.getSize();

    // Verifica se o projétil saiu completamente da janela
    float radius = shape.getRadius();
    return position.x + radius < 0 || position.y + radius < 0 ||
           position.x - radius > windowSize.x || position.y - radius > windowSize.y;
}

sf::FloatRect Projectile::getBounds() const
{
    return shape.getGlobalBounds();
}
