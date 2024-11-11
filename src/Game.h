#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "Constants.h"

class Player;
class Game;
class GameInput;
class Vampire;
class Chest;

namespace sf { class Clock; }

class Game : public sf::Drawable
{
public:
    
    enum class State
    {
        WAITING,
        ACTIVE,
        ENDGAME,
        PAUSE,
    };
    
    Game();
    ~Game();
    
    bool initialise();
    void update(float deltaTime);
    void resetLevel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
    State getState() const { return m_state; }
    
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);

    Player* getPlayer() const;
    sf::Texture* getPlayerTexture() { return &m_playerTexture; }
    sf::Texture* getVampireTexture() { return &m_vampTexture; }
    sf::Texture* getChestTexture() { return &m_chestTexture; }
    sf::Texture* getCoinTexture() { return &m_coinTexture; }

    void vampireSpawner(float deltaTime);
    void chestSpawner(float deltaTime);
    void resetVampireSpawner();
    void start() { m_start = true; };
    void reset() { m_reset = true; };
    void pause();

private:
    std::unique_ptr<Player> m_pPlayer;

    std::vector<std::unique_ptr<Vampire>> m_pVampires;
    std::vector<std::unique_ptr<Chest>> m_pChests;

    State m_state;
    State m_lastState;
    std::unique_ptr<sf::Clock> m_pClock;
    std::unique_ptr<GameInput> m_pGameInput;

    float m_vampireCooldown = 0.0f;
    float m_nextVampireCooldown = 2.0f;
    float m_chestCooldown = 2.0f;
    int m_spawnCount = 0;
    bool m_start = false;
    bool m_reset = false;
    int m_killCount = 0;
    int m_chestCount = 0;
    int m_survivedtime = 0;
    
    sf::Font m_font;
    sf::Texture m_vampTexture;
    sf::Texture m_playerTexture;
    sf::Texture m_chestTexture;
    sf::Texture m_coinTexture;
};
