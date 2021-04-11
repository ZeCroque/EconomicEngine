#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

#include "Grid.h"

class Actor;

class GameManager : public Singleton<GameManager>
{

friend class Singleton<GameManager>;
	
public:
    ~GameManager();
    void Exec();

private:
    GameManager();

    void ProcessInput();
    void Update(float deltaTime);
    void Render();

    std::unique_ptr<sf::RenderWindow> window;
    static const sf::Int32 maxFPS;

    std::list<std::shared_ptr<Actor>> actors;
	Grid grid;
    
};

#endif // GAME_H