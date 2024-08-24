#include "Enemy.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(sf::Vector2f spawnPosition, sf::Vector2f basePosition)
    : basePosition(basePosition), hasReachedBase(false), speed(20.0f)
{
    shape.setRadius(7.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(spawnPosition);
    initialFireDelay = static_cast<float>(std::rand() % 2000) / 1000.0f;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    calculateDirection();
}

void Enemy::calculateDirection()
{
    sf::Vector2f currentPosition = shape.getPosition();
    sf::Vector2f delta = basePosition - currentPosition;
    float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
    direction = delta / distance;
}

void Enemy::update(float deltaTime)
{
    sf::Vector2f currentPosition = shape.getPosition();
    sf::Vector2f delta = basePosition - currentPosition;
    float distance = sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance > speed * deltaTime)
    {
        sf::Vector2f direction = delta / distance;
        shape.move(direction * speed * deltaTime);
    }
    else
    {
        hasReachedBase = true;
    }
}

void Enemy::setBasePosition(const sf::Vector2f &newBasePosition)
{
    basePosition = newBasePosition;
    calculateDirection();
}

sf::Vector2f Enemy::getPosition() const
{
    return shape.getPosition();
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

bool Enemy::reachedBase() const
{
    return hasReachedBase;
}

sf::FloatRect Enemy::getBounds() const
{
    return shape.getGlobalBounds();
}

void Enemy::fireProjectile(std::vector<Projectile> &enemyProjectiles, sf::Vector2f targetPosition)
{
    if (fireClock.getElapsedTime().asSeconds() >= initialFireDelay)
    {
        if (fireClock.getElapsedTime().asSeconds() >= initialFireDelay + 2.0f)
        {
            // Cria um novo projétil na posição do inimigo e direciona-o para o herói
            enemyProjectiles.push_back(Projectile(shape.getPosition(), targetPosition, sf::Color::Red));
            fireClock.restart();
        }
    }
}
