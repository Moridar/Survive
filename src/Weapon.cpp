#include "Weapon.h"
#include "Constants.h"

Weapon::Weapon() : Rectangle(sf::Vector2f(0, 0))
{   
    setColor(sf::Color::Blue);
    setSize(sf::Vector2f(0.0f, 0.0f));
}

void Weapon::update(float deltaTime)
{
    getRotation() == 0.0f || getRotation() == 180.0f ? setSize(sf::Vector2f(WeaponWidth, WeaponHeight))
            : setSize(sf::Vector2f(WeaponHeight, WeaponWidth));
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape graphicsRect(getSize());
    graphicsRect.setFillColor(getColor());
    graphicsRect.setPosition(getPosition());
    target.draw(graphicsRect);
}