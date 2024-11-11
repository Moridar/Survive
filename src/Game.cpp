#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Rectangle.h"
#include "Vampire.h"
#include "Chest.h"

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_vampireCooldown(0.0f),
    m_nextVampireCooldown(0.1f)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
}

bool Game::initialise()
{
    /*
        PAY ATTENTION HIVER!
            If you want to load any assets (fonts, textures) please use the pattern shown below
    */

    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_chestTexture.loadFromFile(ResourceManager::getFilePath("chest.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    resetLevel();
    return true;
}

void Game::resetLevel()
{
    m_pVampires.clear();
    m_pChests.clear();
    m_pPlayer->getWeapon()->setSize(sf::Vector2f(0.0f, 0.0f));

    m_pPlayer->initialise();
    m_pClock->restart();
    resetVampireSpawner();
    m_start = false;
    m_killCount = 0;
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            m_pGameInput->update(deltaTime);
            if (m_start)
            {
                m_state = State::ACTIVE;
                m_pClock->restart();
            }
        }
        break;
            
        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->update(deltaTime);

            vampireSpawner(deltaTime);
            chestSpawner(deltaTime);
            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
            }
            for (auto& tmp : m_pChests)
            {
                tmp->update(deltaTime);
            }

            if (m_pPlayer->isDead())
            {
                m_survivedtime = (int)m_pClock->getElapsedTime().asSeconds();
                m_reset = false;
                m_state = State::ENDGAME;
            }
        }
        break;

        case State::ENDGAME:
        {
            if (m_reset)
            {
                resetLevel();
                m_state = State::WAITING;
            }
        }
        break;
    }

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            m_killCount++;
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            continue;
        }
        i++;
    }
    i = 0;
    while (i < m_pChests.size())
    {
        if (m_pChests[i]->isPicked())
        {
            m_chestCount++;
            std::swap(m_pChests[i], m_pChests.back());
            m_pChests.pop_back();
            continue;
        }
        i++;
    }
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{


    sf::Text defaultText;
    defaultText.setFont(m_font);
    defaultText.setFillColor(sf::Color::White);
    defaultText.setStyle(sf::Text::Bold);
    if (m_state == State::WAITING)
    {
        sf::Text startText(defaultText);
        startText.setString("Survive");
        startText.setPosition(sf::Vector2f((ScreenWidth - startText.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.25));
        target.draw(startText);
        sf::Text pressToStartText(defaultText);
        pressToStartText.setString("Press Space to Start");
        pressToStartText.setPosition(sf::Vector2f((ScreenWidth - pressToStartText.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.75));
        target.draw(pressToStartText);
        startText.setString("Tip: Hold Space to lock weapon direction");
        startText.setPosition(sf::Vector2f((ScreenWidth - startText.getLocalBounds().getSize().x) * 0.5, pressToStartText.getPosition().y + pressToStartText.getLocalBounds().getSize().y * 1.5));
        target.draw(startText);
    }
    else if (m_state == State::ENDGAME)
    {
        sf::Text endText(defaultText), scoreText(defaultText), pressToStartText(defaultText), timerText(defaultText), killCountText(defaultText), chestCountText(defaultText);

        endText.setString("Holy moly, you died!");
        endText.setPosition(sf::Vector2f((ScreenWidth - endText.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.25));
        target.draw(endText);

        killCountText.setString("You killed " + std::to_string(m_killCount) + " vampires");
        killCountText.setPosition(sf::Vector2f((ScreenWidth - killCountText.getLocalBounds().getSize().x) * 0.5, endText.getPosition().y + endText.getLocalBounds().getSize().y * 1.5));
        target.draw(killCountText);
        
        timerText.setString("You survived for " + std::to_string(m_survivedtime) + " seconds");
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, killCountText.getPosition().y + killCountText.getLocalBounds().getSize().y * 1.5));
        target.draw(timerText);

        chestCountText.setString("You picked " + std::to_string(m_chestCount) + " chests");
        chestCountText.setPosition(sf::Vector2f((ScreenWidth - chestCountText.getLocalBounds().getSize().x) * 0.5, timerText.getPosition().y + timerText.getLocalBounds().getSize().y  *1.5)); 
        target.draw(chestCountText);

        sf::Text score(defaultText);
        score.setFillColor(sf::Color::Red);
        score.setString(std::to_string(m_killCount * 10 + m_survivedtime * 1 + m_chestCount * 5));
        score.setPosition(sf::Vector2f((ScreenWidth - score.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.5));
        score.setCharacterSize(50);
        target.draw(score);

        scoreText.setString("Your score is");
        scoreText.setPosition(sf::Vector2f((ScreenWidth - scoreText.getLocalBounds().getSize().x) * 0.5, score.getPosition().y - score.getLocalBounds().getSize().y * 1.5));
        target.draw(scoreText);

        pressToStartText.setString("Press Enter to Restart");
        pressToStartText.setPosition(sf::Vector2f((ScreenWidth - pressToStartText.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.75));
        target.draw(pressToStartText);
        return ;
    }
    else if (m_state == State::PAUSE)
    {
        sf::Text pauseText(defaultText);
        pauseText.setString("Paused");
        pauseText.setPosition(sf::Vector2f((ScreenWidth - pauseText.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.25));
        target.draw(pauseText);
        sf::Text pressToStartText(defaultText);
        pressToStartText.setString("Press Escape to Resume");
        pressToStartText.setPosition(sf::Vector2f((ScreenWidth - pressToStartText.getLocalBounds().getSize().x) * 0.5, ScreenHeight * 0.50));
        target.draw(pressToStartText);
        return ;
    }
    else
    {
        sf::Text timerText(defaultText);
        timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);
        sf::Text killCountText(defaultText);
        killCountText.setString("Kills: " + std::to_string(m_killCount));
        killCountText.setPosition(sf::Vector2f(ScreenWidth - killCountText.getLocalBounds().getSize().x - 10, 20));
        target.draw(killCountText);
    }


    // Draw player.
    m_pPlayer->draw(target, states);
    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }
    for (auto& tmp : m_pChests)
    {
        tmp->draw(target, states);
    }
}


void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

Player* Game::getPlayer() const 
{
    return m_pPlayer.get();
}

void Game::vampireSpawner(float deltaTime)
{
    if (m_vampireCooldown > 0.0f)
    {
        m_vampireCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    float distToRight = (float) ScreenWidth - randomXPos;
    float distToBottom = (float) ScreenHeight - randomYPos;

    float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
    float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

    if (abs(xMinDist) < abs(yMinDist))
        randomXPos += xMinDist;
    else
        randomYPos += yMinDist;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_nextVampireCooldown > 0.3 && m_spawnCount % 5 == 0)
    {
        m_nextVampireCooldown -= 0.05f;
    }
    m_vampireCooldown = m_nextVampireCooldown;
}

void Game::chestSpawner(float deltaTime)
{
    if (m_chestCooldown > 0.0f)
    {
        m_chestCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    m_pChests.push_back(std::make_unique<Chest>(this, spawnPosition));
    m_chestCooldown = m_nextVampireCooldown * 10;
}

void Game::resetVampireSpawner()
{
    m_vampireCooldown = 0.0f;
    m_nextVampireCooldown = 1.0f;
    m_spawnCount = 0;
}

void Game::pause()
{
    if (m_state == State::PAUSE)
    {
        m_state = State::ACTIVE;
    }
    else if (m_state == State::ACTIVE)
    {
        m_state = State::PAUSE;
    }
}