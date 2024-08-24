#include "AmmoBox.hpp"
#include <cmath>

AmmoBox::AmmoBox(sf::Vector2f position)
{
    boxShape.setSize(sf::Vector2f(15.0f, 10.0f)); // Munição
    boxShape.setFillColor(sf::Color::Yellow);
    boxShape.setPosition(position);
}

void AmmoBox::draw(sf::RenderWindow &window)
{
    window.draw(boxShape);
}

// Verifica se o tempo da caixa já passou de 5 segundos
bool AmmoBox::shouldBeDestroyed()
{
    return creationClock.getElapsedTime().asSeconds() >= 5.0f;
}

sf::FloatRect AmmoBox::getBounds() const
{
    return boxShape.getGlobalBounds();
}

void AmmoBox::updateBlinkingEffect()
{
    float lifetime = creationClock.getElapsedTime().asSeconds();

    if (lifetime >= 3.0f) // Inicia o piscar nos últimos 2 segundos
    {
        // Alterna a opacidade com base no tempo restante (sinusoidal para piscar)
        float alpha = 255 * (0.5f + 0.5f * std::sin(10 * (lifetime - 3.0f)));
        sf::Color currentColor = boxShape.getFillColor();
        currentColor.a = static_cast<sf::Uint8>(alpha);
        boxShape.setFillColor(currentColor);
    }
}