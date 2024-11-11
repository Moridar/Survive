#include "Player.h"
#include "Weapon.h"
#include "Whirlwind.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"

#include <iostream>

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>()),
    m_pWhirlwind(std::make_unique<Whirlwind>())
{}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2 - getSize().x / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    
    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;

    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;
    
    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
    setPosition(std::clamp(getPosition().x, 0.0f, (float)ScreenWidth), getPosition().y);

    if (m_pWeapon->isLock() == false)
    {
        if (inputData.m_movingLeft == true && inputData.m_movingRight == false)
            m_direction = LEFT;
        else if (inputData.m_movingLeft == false && inputData.m_movingRight == true)
            m_direction = RIGHT;
        else if (inputData.m_movingUp == true && inputData.m_movingDown == false)
            m_direction = UP;
        else if (inputData.m_movingUp == false && inputData.m_movingDown == true)
            m_direction = DOWN;
        m_pWeapon->setRotation(m_direction == LEFT ? 180.0f : m_direction == UP ? 90.0f : m_direction == DOWN ? 270.0f : 0.0f);
    }
    
}

void Player::buff()
{
    int randomBuff = rand() % 2;
    if (randomBuff == 0)
    {
        m_pWeapon->setLock(true);
        m_pWhirlwind->setActive(true);
    }
    if (randomBuff == 1)
    {
        invisBuff.isActive = true;
        invisBuff.Timer += 5.0f;
        m_sprite.setColor(invisBuff.invisible);
    }
}

void Player::update(float deltaTime)
{
    
    if(m_pWhirlwind->isActive())
    {
        m_pWhirlwind->getBuffTimer() -= deltaTime;
        m_pWhirlwind->getWhirlwindTimer() -= deltaTime;
        m_pWhirlwind->setPosition(sf::Vector2f(getCenter().x - m_pWhirlwind->getSize().x / 2.0f, getCenter().y - m_pWhirlwind->getSize().y / 2.0f));
        if (m_pWhirlwind->getWhirlwindTimer() <= 0.0f)
        {
            m_direction = m_direction == LEFT ? UP : m_direction == UP ? RIGHT : m_direction == RIGHT ? DOWN : LEFT;
            m_pWeapon->setRotation(m_direction == LEFT ? 180.0f : m_direction == UP ? 90.0f : m_direction == DOWN ? 270.0f : 0.0f);
            m_pWhirlwind->getWhirlwindTimer() = 0.05f;
        }
        if (m_pWhirlwind->getBuffTimer() <= 0.0f)
        {
            m_pWhirlwind->setActive(false);
            m_pWeapon->setLock(false);
        }
    }
    if (invisBuff.isActive)
    {
        invisBuff.Timer -= deltaTime;
        if (invisBuff.Timer <= 1.0f)
        {
            m_sprite.setColor(sf::Color(255, 255, 255, 60 - 60 * invisBuff.Timer));
        }
        if (invisBuff.Timer <= 0.0f)
        {
            invisBuff.isActive = false;
            m_sprite.setColor(invisBuff.visible);
        }
    }
    
    sf::Vector2f weaponSize = m_pWeapon->getSize();

    m_sprite.setPosition(getPosition());
    m_pWeapon->setPosition(sf::Vector2f(
        getCenter().x - (m_direction == LEFT ? weaponSize.x : m_direction == RIGHT ? 0.0f : weaponSize.x / 2.0f),
        getCenter().y - (m_direction == UP ? weaponSize.y : m_direction == DOWN ? 0.0f : weaponSize.y / 2.0f)));
    m_pWeapon->update(deltaTime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
}