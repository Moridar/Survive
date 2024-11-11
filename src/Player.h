#pragma once

#include "Rectangle.h"
#include "Weapon.h"
#include "Whirlwind.h"
#include <memory>

struct InputData;

class Game;
class Weapon;
class Whirlwind;

struct s_invisibleBuff
{
    bool isActive = false;
    float Timer = 0.0f;
    sf::Color invisible = sf::Color(255, 255, 255, 0);
    sf::Color visible = sf::Color(255, 255, 255, 255);
};

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}
    
    bool initialise();
    void move(InputData inputData, float deltaTime);
    void buff();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = invisBuff.isActive ? false : isDead; }

    Weapon* getWeapon() { return m_pWeapon.get(); }
    Whirlwind* getWhirlwind() { return m_pWhirlwind.get(); }

private:
    bool    m_isDead = false;
    eDirection m_direction = LEFT;
    Game*   m_pGame;
    std::unique_ptr<Weapon> m_pWeapon;
    std::unique_ptr<Whirlwind> m_pWhirlwind;
    s_invisibleBuff invisBuff;
};
