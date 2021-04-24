#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <nlohmann/json_fwd.hpp>
#include <filesystem>

#include "AbstractFactory.h"
#include "GridManager.h"
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
    void init(const char *contentPath);

    void exec();

    bool getIsRunning() const;

    void setBackgroundNeedsUpdate(bool value);

    const sf::Texture &getTexture(size_t textureId) const;

	bool getIsInitialized() const;

    static const sf::Int32 maxFPS;

private:
    GameManager();

    void initEconomicEngine(const char *prefabsPath);

	void initGui();

    void initMovableTraders(std::vector<nlohmann::json> &parsedMovableTraders);

    void initWorkshops(std::vector<nlohmann::json> &parsedWorkshops);

    void initTexture(const std::filesystem::path &path) const;

    void processInput();

    void update(float deltaTime);

    void render() const;

    void quit();

    std::unique_ptr<sf::RenderWindow> window;
    sf::View view;
    sf::Vector2f oldPos;
    bool moving;

    std::unique_ptr<std::thread> debugGuiThread;	
	
	bool isInitialized;
    bool isRunning;
    bool isGuiOpened;

    mutable bool backgroundNeedsUpdate;
    mutable std::map<size_t, sf::Texture> texturesDictionary;
    mutable sf::RenderTexture background;
    size_t grassId;
    float caseSize;

//SLOTS
//=======
private:
	void traderAddedCallback(class Trader* trader);
	
//GAMEPLAY
//========
public:
    std::shared_ptr<Workshop> addWorkshop(size_t key) const;

    std::shared_ptr<Workshop> addWorkshop(const std::string &name) const;

    std::shared_ptr<MovableTrader> addMovableTrader(size_t key) const;

    std::shared_ptr<MovableTrader> addMovableTrader(const std::string &name) const;

    Workshop *findAvailableWorkshop(size_t jobId) const;

private:
    GridManager gridManager;

    std::queue<MovableTrader *> pendingTraders;
    mutable std::vector<std::shared_ptr<MovableTrader>> traders;
    MovableTraderFactory movableTraderFactory;

    mutable std::vector<std::shared_ptr<Workshop>> workshops;
    WorkshopFactory workshopFactory;

};

#endif //GAME_H
