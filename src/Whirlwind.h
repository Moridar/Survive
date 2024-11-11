#pragma once

#include "Rectangle.h"

class Whirlwind : public Rectangle
{
public:
    Whirlwind();
    virtual ~Whirlwind() {}

    void update(float deltaTime);
    void setActive(bool active);
    bool isActive() { return m_active; }
    float & getBuffTimer() { return m_buffTimer; }
    float & getWhirlwindTimer() { return m_whirlwindTimer; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    bool m_active = false;
    float m_whirlwindTimer = 0.05f;
    float m_buffTimer = 0.0f;
};