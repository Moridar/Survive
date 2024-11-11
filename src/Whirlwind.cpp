#include "Whirlwind.h"
#include "Constants.h"

Whirlwind::Whirlwind() : Rectangle(sf::Vector2f(0, 0))
{
    setColor(sf::Color::Transparent);
}

void Whirlwind::update(float deltaTime)
{
}

void Whirlwind::setActive(bool active)
{
    m_active = active;
    if (active)
    {
        m_buffTimer += 5.0f;
        m_whirlwindTimer = 0.05f;
        setSize(sf::Vector2f(WeaponWidth*1.6, WeaponWidth*1.6));
    }
    else
    {
        m_buffTimer = 0.0f;
        m_whirlwindTimer = 0.0f;
        setSize(sf::Vector2f(0, 0));
    }
}
void Whirlwind::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape graphicsRect(getSize());
    graphicsRect.setFillColor(getColor());
    graphicsRect.setPosition(getPosition());
    target.draw(graphicsRect);
}