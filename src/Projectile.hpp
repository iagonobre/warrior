#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>

class Projectile
{
public:
    Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition, sf::Color color = sf::Color::Blue);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window);

    bool isOutOfBounds(const sf::RenderWindow &window) const;
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;

    void calculateDirection(sf::Vector2f startPosition, sf::Vector2f targetPosition);
};

#endif // PROJECTILE_HPP
