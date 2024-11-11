#pragma once

#include "Rectangle.h"

class Game;

class Chest : public Rectangle
{
    public:
        Chest(Game* game, sf::Vector2f position);
        virtual ~Chest() {}
        
        void update(float deltaTime);

        void setIsPicked(bool isPicked) { m_isPicked = isPicked; }
        bool isPicked() { return m_isPicked; }
    
    private:
        Game* m_pGame;
        bool m_isPicked = false;

};