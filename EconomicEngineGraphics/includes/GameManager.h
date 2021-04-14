#ifndef GAME_H
#define GAME_H

#include <any>

#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

#include "Observer.h"
#include "GridManager.h"

class Actor;

class GameManager : public Singleton<GameManager>, public IObserver
{

friend class Singleton<GameManager>;
	
public:
    ~GameManager() override;
    void exec();

private:
    GameManager();

    void ProcessInput();
    void Update(float deltaTime);
    void Render();

    void quit();

	void guiCloseSignalCallback(std::any lhs);
	void notify(Observable* sender) override;

	std::unique_ptr<std::thread> economicEngineThread;
    std::unique_ptr<std::thread> debugGUIThread;
	
    std::unique_ptr<sf::RenderWindow> window;
    static const sf::Int32 maxFPS;

    std::list<std::shared_ptr<Actor>> actors;
    GridManager gridManager;
	bool economicEngineInitialized;
    bool isGuiOpened;
};

#endif // GAME_H
