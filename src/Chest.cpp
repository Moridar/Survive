#include "Chest.h"
#include "Constants.h"
#include "Game.h"
#include "Player.h"

Chest::Chest(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(ChestWidth, ChestHeight)),
    m_pGame(game)
{
    setPosition(position);
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    setIsPicked(false);

    m_sprite.setTexture(*m_pGame->getChestTexture());
    m_sprite.setScale(2.0f, 2.0f);
    m_sprite.setPosition(getPosition());
}

void Chest::update(float deltaTime)
{
    if (m_isPicked)
        return;

    Player* pPlayer = m_pGame->getPlayer();

    if (collidesWith(pPlayer) || collidesWith(pPlayer->getWeapon()))
    {
        setIsPicked(true);
        pPlayer->buff();
    }
}