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

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    // Gerenciamento de munição
    int getAmmo() const;
    void useAmmo(int amount);
    void addAmmo(int amount);

private:
    sf::CircleShape shape;
    sf::Vector2f targetPosition;
    bool isMoving;
    float speed;
    int ammo;
};

#endif // WARRIOR_HPP
