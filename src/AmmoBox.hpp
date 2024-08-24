#include <SFML/Graphics.hpp>

class AmmoBox
{
public:
    AmmoBox(sf::Vector2f position);

    void draw(sf::RenderWindow &window);
    sf::FloatRect getBounds() const;
    bool shouldBeDestroyed();
    void updateBlinkingEffect();

private:
    sf::RectangleShape boxShape;
    sf::Clock creationClock;
};
