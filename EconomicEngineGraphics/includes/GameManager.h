#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

#include "Grid.h"
#include "Observer.h"

class Actor;

class GameManager : public Singleton<GameManager>, public IObserver
{

friend class Singleton<GameManager>;
	
public:
    ~GameManager() override;
    void Exec();

private:
    GameManager();

    void ProcessInput();
    void Update(float deltaTime);
    void Render();

    void quit();
	
	void notify(Observable* sender) override;

	class EconomicEngineDebugGui** debugGUI;
	std::unique_ptr<std::thread> economicEngineThread;
    std::unique_ptr<std::thread> debugGUIThread;
	
    std::unique_ptr<sf::RenderWindow> window;
    static const sf::Int32 maxFPS;

    std::list<std::shared_ptr<Actor>> actors;
	Grid grid;
    
};

#endif // GAME_H