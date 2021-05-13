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
#include "MovableTrader.h"

using MovableTraderFactory = AbstractFactory<size_t, MovableTrader>;

class GameManager : public Singleton<GameManager>
{
    friend class Singleton<GameManager>;

//FRAMEWORK
//==========
public:
    void init(const char *inContentPath);

    void exec();

    bool getIsRunning() const;

    void setBackgroundNeedsUpdate(bool inValue) const;

	void setSpeedFactor(float inSpeedFactor);

	[[nodiscard]] float getSpeedFactor() const;

    const sf::Texture &getTexture(size_t inTextureId) const;

	bool getIsInitialized() const;


    static const sf::Int32 maxFPS;
	float speedFactor;

private:
    GameManager();

    void initEconomicEngine(const char *inPrefabsPath);

	void initGui();

    void initMovableTraders(std::vector<nlohmann::json> &inParsedMovableTraders);

    void initWorkshops(std::vector<nlohmann::json> &inParsedWorkshops);

    void initTexture(const std::filesystem::path &inPath) const;

    void processInput();

    void update(float inDeltaTime);

    void render() const;

    void getSelectedActor();

    void quit();

    std::unique_ptr<sf::RenderWindow> window;
    sf::View view;
    sf::Vector2f oldPos;
    sf::Vector2f clickPos;
    bool moving;
    bool drawPopup;

    std::unique_ptr<std::thread> debugGuiThread;	
	
	bool isInitialized;
    bool isRunning;
	bool isGuiOpened;
    bool wantsToOpenGui;

    mutable bool backgroundNeedsUpdate;
    mutable std::map<size_t, sf::Texture> texturesDictionary;
    mutable sf::RenderTexture background;
    Workshop* selectedActor;
    size_t grassId;
    float caseSize;

//SLOTS
//=======
private:
	void traderAddedCallback(class Trader* inTrader);
	
//GAMEPLAY
//========
public:
    std::shared_ptr<Workshop> addWorkshop(size_t inKey) const;

    std::shared_ptr<Workshop> addWorkshop(const std::string &inName) const;

    std::shared_ptr<MovableTrader> addMovableTrader(size_t inKey) const;

    std::shared_ptr<MovableTrader> addMovableTrader(const std::string &inName) const;

    Workshop *findAvailableWorkshop(size_t inJobId) const;

	[[nodiscard]] const GridManager& getGridManager() const;

private:
    GridManager gridManager;

    std::queue<MovableTrader *> pendingTraders;
    mutable std::list<std::shared_ptr<MovableTrader>> traders;
    MovableTraderFactory movableTraderFactory;

    mutable std::list<std::shared_ptr<Workshop>> workshops;
    WorkshopFactory workshopFactory;

};

#endif //GAME_H
