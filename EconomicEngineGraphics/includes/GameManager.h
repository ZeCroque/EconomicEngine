#ifndef GAME_H
#define GAME_H

#include <any>

#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <queue>
#include <nlohmann/json_fwd.hpp>


#include "AbstractFactory.h"
#include "Observer.h"
#include "GridManager.h"

class Workshop;
class MovableTrader;

class GameManager : public Singleton<GameManager>, public IObserver {

    friend class Singleton<GameManager>;

public:
    ~GameManager() override;
	
    void init(const char* prefabsPath);
	
    void exec();

private:
    GameManager();

	void initThreads(const char* prefabsPath);

	void initMovableTraders(std::vector<nlohmann::json>& parsedJobs);

	void initWorkshops(std::vector<nlohmann::json>& parsedWorkshops);
	
    void processInput();

    void update(float deltaTime);

    void render() const;

    void quit() const;

    void guiCloseSignalCallback(std::any lhs);
    
    void notify(Observable *sender) override;

    Workshop* findAvailableWorkshop(size_t jobId);
	
    std::unique_ptr<std::thread> economicEngineThread;
    std::unique_ptr<std::thread> debugGUIThread;

    std::unique_ptr<sf::RenderWindow> window;
    static const sf::Int32 maxFPS;

    std::list<std::shared_ptr<Actor>> actors;
    GridManager gridManager;
    bool economicEngineInitialized;
    bool isGuiOpened;

    AbstractFactory<size_t, MovableTrader> movableTraderFactory;
	AbstractFactory<size_t, Workshop> workshopFactory;
	
    std::queue<MovableTrader*> pendingTraders;
    std::vector<std::shared_ptr<MovableTrader>> traders;
    std::vector<std::shared_ptr<Workshop>> workshops;


};

#endif // GAME_H
