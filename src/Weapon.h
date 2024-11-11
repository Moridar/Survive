#pragma once

#include "Rectangle.h"

enum eDirection
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Weapon : public Rectangle
{
public:
    Weapon();
    virtual ~Weapon() {}

    void update(float deltaTime);
    bool isLock() { return m_isLock; }
    void setLock(bool isLock) { m_isLock = isLock; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    bool m_isLock = false;
};