#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Warrior.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include <vector>

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void spawnEnemy();
    void fireProjectile();
    void updateEnemyProjectiles(float deltaTime);
    void updateWarriorProjectiles(float deltaTime);

    sf::RenderWindow window;
    Warrior warrior;

    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    sf::RectangleShape base;
    std::vector<Projectile> enemyProjectiles;

    // Textos
    sf::Font font;
    sf::Text ammoText;
    sf::Text baseLifeText;
    sf::Text killsText;
    sf::Text timerText;
    sf::Text heroLifeText;

    // Variáveis de jogabilidade
    int heroLife = 100;
    int baseLife = 100;
    int kills = 0;
    const sf::Time fireDelay = sf::seconds(0.2f);
    const sf::Time spawnInterval = sf::seconds(2);

    // Timers
    sf::Clock clock;
    sf::Clock victoryClock;
    sf::Clock baseRegenClock;
    sf::Time timeSinceLastHit;
    sf::Time timeSinceLastSpawn;
    sf::Time timeSinceLastShot;

    // Variável de controle de jogo
    bool isPaused = false;
    bool gameOver = false;
    bool gameWon = false;
};

#endif // GAME_HPP
