#include "Game.hpp"
#include "AmmoBox.hpp"
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.hpp"

std::vector<AmmoBox> ammoBoxes;

Game::Game()
    : window(sf::VideoMode(960, 500), "Guardians of the Core", sf::Style::Default, sf::ContextSettings(0, 0, 8)),
      warrior(10.0f, sf::Vector2f(480.0f, 250.0f)),
      timeSinceLastSpawn(sf::Time::Zero)
{
    window.setVerticalSyncEnabled(true);

    base.setSize(sf::Vector2f(200.0f, 100.0f));
    base.setFillColor(sf::Color(240, 240, 240));
    base.setOutlineThickness(5.0f);
    base.setOutlineColor(sf::Color(0, 100, 0));
    base.setPosition(480.0f - 100.0f, 250.0f - 50.0f);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Carregar a fonte
    if (!font.loadFromFile("font.ttf"))
    {
        std::cout << "Erro ao carregar a fonte!" << std::endl;
    }

    // Textos

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

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f newPosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    warrior.moveTo(newPosition);
                }
            }

            // Dispara projéteis com a tecla "Q"
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
            {
                if (timeSinceLastShot >= fireDelay)
                {
                    fireProjectile();
                    timeSinceLastShot = sf::Time::Zero;
                }
            }

            // Lida com o aumento da tela
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                float newWidth = static_cast<float>(event.size.width);
                float newHeight = static_cast<float>(event.size.height);

                float baseWidth = base.getSize().x;
                float baseHeight = base.getSize().y;

                // Reposicionar a base no centro da tela
                base.setPosition((newWidth - baseWidth) / 2, (newHeight - baseHeight) / 2);

                // Atualizar a posição da base para todos os inimigos existentes
                sf::Vector2f newBasePosition = base.getPosition() + sf::Vector2f(baseWidth / 2, baseHeight / 2);
                for (Enemy &enemy : enemies)
                {
                    enemy.setBasePosition(newBasePosition);
                }
                warrior.setPosition(newBasePosition);

                // Atualiza a posição do texto no canto superior direito
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
        pauseVictoryTime = victoryClock.getElapsedTime();
        clock.restart();
        return;
    }
    else
    {
        totalVictoryTime += victoryClock.restart();
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

    // Regenerar a vida da base a cada 5 segundos sem levar dano
    if (timeSinceLastHit >= sf::seconds(5.0f) && baseLife < 100)
    {
        baseLife += 1;
        timeSinceLastHit = sf::Time::Zero;

        float lifePercentage = static_cast<float>(baseLife) / 100.0f;
        sf::Color baseColor = sf::Color(
            static_cast<sf::Uint8>(255 * (1.0f - lifePercentage)), // Mais vermelho
            static_cast<sf::Uint8>(100 * lifePercentage),          // Menos verde
            0                                                      // Sem azul
        );
        base.setOutlineColor(baseColor);
    }

    warrior.update(deltaTime);

    heroLifeText.setString("VIDA: " + std::to_string(heroLife) + " | 100");
    ammoText.setString("MUNICAO: " + std::to_string(warrior.getAmmo()) + " | 100");
    baseLifeText.setString("BASE: " + std::to_string(baseLife) + " %");
    killsText.setString("KILLS: " + std::to_string(kills));
    timerText.setString(std::to_string(static_cast<int>(timeLeft)) + "s ATE VITORIA");

    for (Enemy &enemy : enemies)
    {
        enemy.update(deltaTime);
        enemy.fireProjectile(enemyProjectiles, warrior.getPosition());
    }
    // Atualiza os projéteis do guerreiro
    updateWarriorProjectiles(deltaTime);

    // Atualiza os projéteis dos inimigos
    updateEnemyProjectiles(deltaTime);

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        it->update(deltaTime);

        sf::Vector2f enemyPosition = it->getPosition();
        sf::FloatRect baseBounds = base.getGlobalBounds();

        if (baseBounds.contains(enemyPosition))
        {
            baseLife -= 5;
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Atualiza as caixas de munição
    for (auto it = ammoBoxes.begin(); it != ammoBoxes.end();)
    {

        it->updateBlinkingEffect();

        // Se o herói tocar na caixa de munição
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
        pauseText.setString("PAUSADO");

        sf::Vector2u windowSize = window.getSize();

        sf::FloatRect textBounds = pauseText.getLocalBounds();

        pauseText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);                                  // Centralizar o texto
        pauseText.setPosition(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) / 2.0f); // Posição central

        if (!gameOver && !gameWon)
        {
            pauseText.setString("PAUSADO");
        }
        else if (gameWon)
        {
            pauseText.setString("VITORIA!");
        }
        else
        {
            pauseText.setString("GAME OVER");
        }
        window.draw(pauseText);
    }

    window.display();
}

void Game::spawnEnemy()
{
    // Spawna o inimigo em uma das bordas aleatórias
    int edge = std::rand() % 4;
    sf::Vector2f spawnPosition;

    switch (edge)
    {
    case 0: // Borda superior
        spawnPosition = sf::Vector2f(static_cast<float>(std::rand() % window.getSize().x), 0.0f);
        break;
    case 1: // Borda inferior
        spawnPosition = sf::Vector2f(static_cast<float>(std::rand() % window.getSize().x), static_cast<float>(window.getSize().y));
        break;
    case 2: // Borda esquerda
        spawnPosition = sf::Vector2f(0.0f, static_cast<float>(std::rand() % window.getSize().y));
        break;
    case 3: // Borda direita
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
        // Atualiza a posição do projétil
        it->update(deltaTime);

        // Obter os limites do projétil para verificação de colisão
        sf::FloatRect projectileBounds = it->getBounds();
        bool projectileRemoved = false;

        // Colisão com o herói
        if (projectileBounds.intersects(warrior.getBounds()))
        {
            heroLife -= 1;

            // Verificar se a vida do herói chegou a 0
            if (heroLife <= 0)
            {
                gameOver = true;
                isPaused = true;
            }

            it = enemyProjectiles.erase(it);
            projectileRemoved = true;
            continue;
        }

        // Colisão com a base
        if (projectileBounds.intersects(base.getGlobalBounds()))
        {
            // Reduz a vida da base e reseta o temporizador de regeneração
            baseLife -= 1;
            timeSinceLastHit = sf::Time::Zero;

            // Atualiza a cor do contorno conforme a vida cai (de verde para vermelho)
            float lifePercentage = static_cast<float>(baseLife) / 100.0f;
            sf::Color baseColor = sf::Color(
                static_cast<sf::Uint8>(255 * (1.0f - lifePercentage)), // Mais vermelho
                static_cast<sf::Uint8>(100 * lifePercentage),          // Menos verde
                0                                                      // Sem azul
            );
            base.setOutlineColor(baseColor);

            // Remover o projétil ao colidir com a base
            it = enemyProjectiles.erase(it);
            projectileRemoved = true;
            continue;
        }

        // Verifica se o projétil saiu da tela
        if (it->isOutOfBounds(window))
        {
            // Remover o projétil quando sair da tela
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

        // Verifica se o projétil colidiu com algum inimigo
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
        {
            sf::FloatRect enemyBounds = enemyIt->getBounds();

            // Se houver colisão entre o projétil e o inimigo
            if (projectileBounds.intersects(enemyBounds))
            {
                kills++;
                // Pega a posição do inimigo
                sf::Vector2f enemyPosition = enemyIt->getPosition();

                // Remove o inimigo e o projétil
                enemyIt = enemies.erase(enemyIt);
                it = projectiles.erase(it);

                // Adiciona uma caixa de munição na posição do inimigo destruído
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

        // Se o projétil foi removido devido à colisão, não verifica se está fora da tela
        if (!projectileRemoved)
        {
            if (it->isOutOfBounds(window))
            {
                it = projectiles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}
