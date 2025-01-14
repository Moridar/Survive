#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
GameInput::GameInput(Game* pGame, Player* pPlayer) :
    m_pGame(pGame), m_pPlayer(pPlayer) {}


void GameInput::update(float deltaTime)
{
    if (m_inputData.hasInputs())
    {
        m_pPlayer->move(m_inputData, deltaTime);
    }
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up || key == sf::Keyboard::W)
    {
        m_inputData.m_movingUp = true;
    }
    else if (key == sf::Keyboard::Down || key == sf::Keyboard::S)
    {
        m_inputData.m_movingDown = true;
    }
    else if (key == sf::Keyboard::Left || key == sf::Keyboard::A)
    {
        m_inputData.m_movingLeft = true;
    }
    else if (key == sf::Keyboard::Right || key == sf::Keyboard::D)
    {
        m_inputData.m_movingRight = true;
    }
    else if (key == sf::Keyboard::Space)
    {
        m_pGame->start();
        m_pPlayer->getWeapon()->setLock(true);
    }
    else if (key == sf::Keyboard::Enter)
    {
        m_pGame->reset();
    }
    else if (key == sf::Keyboard::Escape)
    {
        m_pGame->pause();
    }
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up || key == sf::Keyboard::W)
    {
        m_inputData.m_movingUp = false;
    }
    else if (key == sf::Keyboard::Down || key == sf::Keyboard::S)
    {
        m_inputData.m_movingDown = false;
    }
    else if (key == sf::Keyboard::Left || key == sf::Keyboard::A)
    {
        m_inputData.m_movingLeft = false;
    }
    else if (key == sf::Keyboard::Right || key == sf::Keyboard::D)
    {
        m_inputData.m_movingRight = false;
    }
    else if (key == sf::Keyboard::Space)
    {
        m_pPlayer->getWeapon()->setLock(false);
    }
}
