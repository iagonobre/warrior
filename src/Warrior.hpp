#ifndef WARRIOR_HPP
#define WARRIOR_HPP

#include <SFML/Graphics.hpp>

class Warrior
{
public:
    Warrior(float radius, sf::Vector2f position);

    void draw(sf::RenderWindow &window);
    void moveTo(sf::Vector2f newPosition);
    void setPosition(sf::Vector2f newPosition);
    void update(float deltaTime);

private:
    sf::CircleShape shape;
    sf::Vector2f targetPosition;
    bool isMoving;
    float speed;
};

#endif // WARRIOR_HPP
