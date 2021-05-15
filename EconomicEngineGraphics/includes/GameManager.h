#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <nlohmann/json_fwd.hpp>
#include <filesystem>

#include "Singleton.h"
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
    void init(const char* inContentPath);

    void exec();

    void pause();
	
	void resume();

	void reset(int inTradersCount);

    bool isRunning() const;

	bool isInitialized() const;

    void setBackgroundNeedsUpdate(bool inValue) const;

	void setSpeedFactor(float inSpeedFactor);

	void setMaxFps(int inMaxFps);

	[[nodiscard]] float getSpeedFactor() const;

	[[nodiscard]] int getMaxFps() const;

    const sf::Texture& getTexture(size_t inTextureId) const;

private:
    GameManager();

    void initEconomicEngine(const char* inPrefabsPath);

	void initGui();

    void initMovableTraders(std::vector<nlohmann::json>& inParsedMovableTraders);

    void initWorkshops(std::vector<nlohmann::json>& inParsedWorkshops);

    void initTexture(const std::filesystem::path& inPath) const;

    void processInput();

    void update(float inDeltaTime);

    void render() const;

    void renderBackground(sf::Vector2<float> viewOrigin, float viewXMin, float viewXMax, float viewYMin,
                          float viewYMax) const;

    void renderTrader(float viewXMin, float viewXMax, float viewYMin, float viewYMax) const;

    void renderPopup() const;

    void quit();

	int maxFps;
    std::unique_ptr<sf::RenderWindow> window;
    sf::View view;
    sf::Font font;
    sf::Vector2f oldPos;
    sf::Vector2f clickPos;
    bool bMoving;
    bool bDrawPopup;

    std::unique_ptr<std::thread> debugGuiThread;	
	
	bool bIsInitialized;
    bool bIsRunning;
	bool bIsPaused;
	bool bIsGuiOpened;
    bool bWantsToOpenGui;
	float speedFactor;

    mutable bool bBackgroundNeedsUpdate;
    mutable std::map<size_t, sf::Texture> texturesDictionary;
    mutable sf::RenderTexture background;
    Workshop* selectedActor;
    size_t grassId;
    size_t popupId;
    float caseSize;

//SLOTS
//=======
private:
	void traderAddedCallback(class Trader* inTrader);
	
//GAMEPLAY
//========
public:
    std::shared_ptr<Workshop> addWorkshop(size_t inKey) const;

    std::shared_ptr<Workshop> addWorkshop(const std::string& inName) const;

    std::shared_ptr<MovableTrader> addMovableTrader(size_t inKey) const;

    std::shared_ptr<MovableTrader> addMovableTrader(const std::string& inName) const;


    Workshop* findAvailableWorkshop(size_t inJobId) const;
	
    void getSelectedActor();

	[[nodiscard]] const GridManager& getGridManager() const;

private:

	void updateWorkshops(float inDeltaTime);

	void updateTraders(float inDeltaTime) const;
	
    GridManager gridManager;

    std::queue<MovableTrader*> pendingTraders;
    mutable std::list<std::shared_ptr<MovableTrader>> traders;
    MovableTraderFactory movableTraderFactory;

    mutable std::list<std::shared_ptr<Workshop>> workshops;
    WorkshopFactory workshopFactory;

};

#endif //GAME_H
