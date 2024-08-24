#include "Game.hpp"
#include "AmmoBox.hpp"
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.hpp"

std::vector<AmmoBox> ammoBoxes; // Vetor global para caixas de munição

Game::Game()
    : window(sf::VideoMode(960, 500), "Guardians of the Core", sf::Style::Default, sf::ContextSettings(0, 0, 8)),
      warrior(10.0f, sf::Vector2f(480.0f, 250.0f)),
      timeSinceLastSpawn(sf::Time::Zero)
{
    window.setVerticalSyncEnabled(true);

    // Configuração da base
    base.setSize(sf::Vector2f(200.0f, 100.0f));
    base.setFillColor(sf::Color(240, 240, 240));
    base.setOutlineThickness(5.0f);
    base.setOutlineColor(sf::Color(0, 100, 0));
    base.setPosition(480.0f - 100.0f, 250.0f - 50.0f);

    // Inicializa o gerador de números aleatórios
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Carrega a fonte para os textos
    if (!font.loadFromFile("font.ttf"))
    {
        std::cout << "Erro ao carregar a fonte!" << std::endl;
    }

    // Configuração dos textos
    heroLifeText.setFont(font);
    heroLifeText.setCharacterSize(10);
    heroLifeText.setFillColor(sf::Color::Black);
    heroLifeText.setPosition(750, 10);

    ammoText.setFont(font);
    ammoText.setCharacterSize(10);
    ammoText.setFillColor(sf::Color::Black);
    ammoText.setPosition(750, 25);

    baseLifeText.setFont(font);
    baseLifeText.setCharacterSize(10);
    baseLifeText.setFillColor(sf::Color::Black);
    baseLifeText.setPosition(750, 40);

    killsText.setFont(font);
    killsText.setCharacterSize(10);
    killsText.setFillColor(sf::Color::Black);
    killsText.setPosition(750, 55);

    timerText.setFont(font);
    timerText.setCharacterSize(10);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(750, 70);
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (!gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
        {
            isPaused = !isPaused;
        }

        if (!isPaused)
        {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f newPosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                warrior.moveTo(newPosition);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q && timeSinceLastShot >= fireDelay)
            {
                fireProjectile();
                timeSinceLastShot = sf::Time::Zero;
            }

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                // Reposicionar base e textos ao aumentar a tela
                float newWidth = static_cast<float>(event.size.width);
                float newHeight = static_cast<float>(event.size.height);
                float baseWidth = base.getSize().x;
                float baseHeight = base.getSize().y;

                base.setPosition((newWidth - baseWidth) / 2, (newHeight - baseHeight) / 2);
                sf::Vector2f newBasePosition = base.getPosition() + sf::Vector2f(baseWidth / 2, baseHeight / 2);

                for (Enemy &enemy : enemies)
                {
                    enemy.setBasePosition(newBasePosition);
                }
                warrior.setPosition(newBasePosition);

                // Atualiza posição dos textos
                heroLifeText.setPosition(newWidth - 200, 10);
                ammoText.setPosition(newWidth - 200, 25);
                baseLifeText.setPosition(newWidth - 200, 40);
                killsText.setPosition(newWidth - 200, 55);
                timerText.setPosition(newWidth - 200, 70);
            }
        }
    }
}

void Game::update()
{
    if (isPaused)
    {
        clock.restart();
        return;
    }

    float deltaTime = clock.restart().asSeconds();
    timeSinceLastSpawn += sf::seconds(deltaTime);
    timeSinceLastShot += sf::seconds(deltaTime);
    timeSinceLastHit += sf::seconds(deltaTime);

    if (baseLife <= 0)
    {
        gameOver = true;
        isPaused = true;
        return;
    }

    float timeElapsed = victoryClock.getElapsedTime().asSeconds();
    float timeLeft = 120.0f - timeElapsed;

    if (timeLeft <= 0.0f)
    {
        gameWon = true;
        isPaused = true;
        return;
    }

    if (timeSinceLastHit >= sf::seconds(5.0f) && baseLife < 100)
    {
        baseLife += 1;
        timeSinceLastHit = sf::Time::Zero;

        float lifePercentage = static_cast<float>(baseLife) / 100.0f;
        sf::Color baseColor = sf::Color(
            static_cast<sf::Uint8>(255 * (1.0f - lifePercentage)),
            static_cast<sf::Uint8>(100 * lifePercentage),
            0);
        base.setOutlineColor(baseColor);
    }

    // Atualiza textos
    heroLifeText.setString("VIDA: " + std::to_string(heroLife) + " | 100");
    ammoText.setString("MUNICAO: " + std::to_string(warrior.getAmmo()) + " | 100");
    baseLifeText.setString("BASE: " + std::to_string(baseLife) + " %");
    killsText.setString("KILLS: " + std::to_string(kills));
    timerText.setString(std::to_string(static_cast<int>(timeLeft)) + "s ATE VITORIA");

    warrior.update(deltaTime);

    for (Enemy &enemy : enemies)
    {
        enemy.update(deltaTime);
        enemy.fireProjectile(enemyProjectiles, warrior.getPosition());
    }

    updateWarriorProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        it->update(deltaTime);
        sf::FloatRect baseBounds = base.getGlobalBounds();

        if (baseBounds.contains(it->getPosition()))
        {
            baseLife -= 5;
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = ammoBoxes.begin(); it != ammoBoxes.end();)
    {
        it->updateBlinkingEffect();

        if (warrior.getBounds().intersects(it->getBounds()))
        {
            warrior.addAmmo(10);
            it = ammoBoxes.erase(it);
        }
        else if (it->shouldBeDestroyed())
        {
            it = ammoBoxes.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (timeSinceLastSpawn >= spawnInterval)
    {
        spawnEnemy();
        timeSinceLastSpawn = sf::Time::Zero;
    }
}

void Game::render()
{
    window.clear(sf::Color::White);

    window.draw(base);
    warrior.draw(window);

    for (Enemy &enemy : enemies)
    {
        enemy.draw(window);
    }

    for (Projectile &projectile : projectiles)
    {
        projectile.draw(window);
    }

    for (Projectile &projectile : enemyProjectiles)
    {
        projectile.draw(window);
    }

    for (AmmoBox &ammoBox : ammoBoxes)
    {
        ammoBox.draw(window);
    }

    window.draw(ammoText);
    window.draw(baseLifeText);
    window.draw(killsText);
    window.draw(timerText);
    window.draw(heroLifeText);

    if (isPaused)
    {
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(50);
        pauseText.setFillColor(sf::Color::Black);
        pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
        pauseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        if (gameWon)
        {
            pauseText.setString("VITORIA!");
        }
        else if (gameOver)
        {
            pauseText.setString("GAME OVER");
        }
        else
        {
            pauseText.setString("PAUSADO");
        }
        window.draw(pauseText);
    }

    window.display();
}

void Game::spawnEnemy()
{
    int edge = std::rand() % 4;
    sf::Vector2f spawnPosition;

    switch (edge)
    {
    case 0:
        spawnPosition = sf::Vector2f(static_cast<float>(std::rand() % window.getSize().x), 0.0f);
        break;
    case 1:
        spawnPosition = sf::Vector2f(static_cast<float>(std::rand() % window.getSize().x), static_cast<float>(window.getSize().y));
        break;
    case 2:
        spawnPosition = sf::Vector2f(0.0f, static_cast<float>(std::rand() % window.getSize().y));
        break;
    case 3:
        spawnPosition = sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(std::rand() % window.getSize().y));
        break;
    }

    sf::Vector2f baseCenter = base.getPosition() + sf::Vector2f(base.getSize().x / 2, base.getSize().y / 2);
    enemies.push_back(Enemy(spawnPosition, baseCenter));
}

void Game::fireProjectile()
{
    if (warrior.getAmmo() > 0)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
        sf::Vector2f startPosition = warrior.getPosition();
        projectiles.push_back(Projectile(startPosition, worldPosition, sf::Color::Blue));
        warrior.useAmmo(1);
    }
}

void Game::updateEnemyProjectiles(float deltaTime)
{
    for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end();)
    {
        it->update(deltaTime);
        sf::FloatRect projectileBounds = it->getBounds();
        bool projectileRemoved = false;

        if (projectileBounds.intersects(warrior.getBounds()))
        {
            heroLife -= 1;
            if (heroLife <= 0)
            {
                gameOver = true;
                isPaused = true;
            }
            it = enemyProjectiles.erase(it);
            projectileRemoved = true;
            continue;
        }

        if (projectileBounds.intersects(base.getGlobalBounds()))
        {
            baseLife -= 1;
            timeSinceLastHit = sf::Time::Zero;

            float lifePercentage = static_cast<float>(baseLife) / 100.0f;
            sf::Color baseColor = sf::Color(
                static_cast<sf::Uint8>(255 * (1.0f - lifePercentage)),
                static_cast<sf::Uint8>(100 * lifePercentage),
                0);
            base.setOutlineColor(baseColor);
            it = enemyProjectiles.erase(it);
            projectileRemoved = true;
            continue;
        }

        if (it->isOutOfBounds(window))
        {
            it = enemyProjectiles.erase(it);
            projectileRemoved = true;
            continue;
        }

        if (!projectileRemoved)
        {
            ++it;
        }
    }
}

void Game::updateWarriorProjectiles(float deltaTime)
{
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        it->update(deltaTime);
        sf::FloatRect projectileBounds = it->getBounds();
        bool projectileRemoved = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
        {
            if (projectileBounds.intersects(enemyIt->getBounds()))
            {
                kills++;
                sf::Vector2f enemyPosition = enemyIt->getPosition();
                enemyIt = enemies.erase(enemyIt);
                it = projectiles.erase(it);

                if (warrior.getAmmo() < 100)
                {
                    ammoBoxes.push_back(AmmoBox(enemyPosition));
                }

                projectileRemoved = true;
                break;
            }
            else
            {
                ++enemyIt;
            }
        }

        if (!projectileRemoved && it->isOutOfBounds(window))
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
