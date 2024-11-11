#include "Vampire.h"
#include "Constants.h"
#include "Game.h"
#include "Weapon.h"
#include "Whirlwind.h"
#include "Player.h"
#include "MathUtils.h"

Vampire::Vampire(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(VampireWidth, VampireHeight)),
    m_pGame(game)
{
    setPosition(position);
    setIsKilled(false);

    m_sprite.setTexture(*m_pGame->getVampireTexture());
    m_sprite.setScale(2.0f, 2.0f);
}

void Vampire::update(float deltaTime)
{
    if (m_isKilled)
        return;
    
    Player* pPlayer = m_pGame->getPlayer();

    if (collidesWith(pPlayer->getWeapon()) || collidesWith(pPlayer->getWhirlwind()))
    {
        setIsKilled(true);
        return;
    }

    if (collidesWith(pPlayer))
        pPlayer->setIsDead(true);

    sf::Vector2f playerCenter = pPlayer->getCenter();
    sf::Vector2f direction = VecNormalized(playerCenter - getCenter());
    direction *= VampireSpeed * deltaTime;
    sf::Transformable::move(direction);
    m_sprite.setPosition(getPosition());
}