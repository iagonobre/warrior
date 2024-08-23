#include "Game.hpp"

Game::Game()
    : window(sf::VideoMode(960, 500), "Jogo que ainda não tem nome", sf::Style::Default, sf::ContextSettings(0, 0, 8, 0, 0, 0, 0)), // Inicializa a janela com antialiasing
      warrior(10.0f, sf::Vector2f(480.0f, 250.0f))
{
    window.setVerticalSyncEnabled(true);
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

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f newPosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                warrior.moveTo(newPosition);
            }
        }

        // Lida com o aumento da tela
        if (event.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));

            // Mover objetos para o centro da tela
            sf::Vector2f newCenter(static_cast<float>(event.size.width) / 2, static_cast<float>(event.size.height) / 2);
            warrior.setPosition(newCenter);
        }
    }
}

void Game::update()
{
    // Calcula o tempo decorrido desde o último frame
    float deltaTime = clock.restart().asSeconds();

    // Atualiza o estado do guerreiro
    warrior.update(deltaTime);
}

void Game::render()
{
    window.clear(sf::Color::White);
    warrior.draw(window);
    window.display();
}
