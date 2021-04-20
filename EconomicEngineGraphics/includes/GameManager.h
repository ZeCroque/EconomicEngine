#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <nlohmann/json_fwd.hpp>

#include "AbstractFactory.h"
#include "GridManager.h"
#include "Signal.h"
#include "WorkshopFactory.h"

class Workshop;
class MovableTrader;

using MovableTraderFactory = AbstractFactory<size_t, MovableTrader>;

class GameManager : public Singleton<GameManager>
{
    friend class Singleton<GameManager>;

//FRAMEWORK
//==========
public:
    void init(const char* prefabsPath);
    void exec();

    bool getIsRunning() const;
    bool getHaveEverRun() const;

private:
    GameManager();
	
	void initEconomicEngine(const char* prefabsPath);
	void initMovableTraders(std::vector<nlohmann::json>& parsedMovableTraders);
	void initWorkshops(std::vector<nlohmann::json>& parsedWorkshops);
	
    void processInput();
    void update(float deltaTime);
    void render() const;
	
    void quit();

	std::unique_ptr<sf::RenderWindow> window;
    static const sf::Int32 maxFPS;
	
	std::unique_ptr<std::thread> debugGuiThread;
	std::unique_ptr<std::thread> economicEngineThread;
	
	bool isInitialized;
    bool isRunning;
    bool haveEverRun;
	bool isGuiOpened;

	
//SLOTS
//=======
private:
	void traderAddedCallback(class Trader* trader);
	void askResolvedCallback();
	
//GAMEPLAY
//========
public:
	std::shared_ptr<Workshop> addWorkshop(size_t key) const;	
	std::shared_ptr<Workshop> addWorkshop(const std::string& name) const;
	std::shared_ptr<MovableTrader> addMovableTrader(size_t key) const;
	std::shared_ptr<MovableTrader> addMovableTrader(const std::string& name) const;
    Workshop* findAvailableWorkshop(size_t jobId) const;

private:
    GridManager gridManager;

	std::queue<MovableTrader*> pendingTraders;
	mutable std::vector<std::shared_ptr<MovableTrader>> traders;
	MovableTraderFactory movableTraderFactory;
	
    mutable std::vector<std::shared_ptr<Workshop>> workshops;
	WorkshopFactory workshopFactory;
};

#endif //GAME_H
