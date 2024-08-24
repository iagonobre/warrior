#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include "Projectile.hpp"

class Enemy
{
public:
    Enemy(sf::Vector2f spawnPosition, sf::Vector2f basePosition);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window);
    void fireProjectile(std::vector<Projectile> &enemyProjectiles, sf::Vector2f heroPosition);
    void setBasePosition(const sf::Vector2f &newBasePosition);

    bool reachedBase() const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;
    bool hasReachedBase;
    sf::Vector2f basePosition;
    sf::Clock fireClock;
    float initialFireDelay;

    void calculateDirection();
};

#endif // ENEMY_HPP
