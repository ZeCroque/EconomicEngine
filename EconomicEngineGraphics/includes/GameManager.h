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

    void start(int inTradersAmount = 50);

    void pause();
	
	void resume();

	void reset(int inTradersAmount);

    bool isRunning() const;

	bool isInitialized() const;

    void setBackgroundNeedsUpdate(bool inValue) const;

	void setSpeedFactor(float inSpeedFactor);

	void setMaxFps(int inMaxFps);

	[[nodiscard]] float getSpeedFactor() const;

	[[nodiscard]] int getMaxFps() const;

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

    void quit();

	int maxFps;
    std::unique_ptr<sf::RenderWindow> window;

    std::unique_ptr<std::thread> debugGuiThread;	
	
	bool bIsInitialized;
    bool bIsRunning;
	bool bIsPaused;
	bool bIsGuiOpened;
    bool bWantsToOpenGui;
	float speedFactor;

//INPUTS
//=========
private:
    void getSelectedActor();
	
    sf::Vector2f oldMousePosition;
    sf::Vector2f mouseClickPosition;
    bool bMouseDragging;
    bool bDrawPopup;

	Workshop* selectedActor;

//RENDERING
//=========
public:
    const sf::Texture& getTexture(size_t inTextureId) const;

private:	
	void renderBackground(sf::Vector2<float> inViewOrigin, float inViewXMin, float inViewXMax, float inViewYMin,
                          float inViewYMax) const;

    void renderTrader(float inViewXMin, float inViewXMax, float inViewYMin, float inViewYMax) const;

    void renderPopup() const;

    sf::View view;
    sf::Font font;
	mutable sf::RenderTexture background;
    mutable bool bBackgroundNeedsUpdate;
	
    mutable std::map<size_t, sf::Texture> texturesDictionary;
    size_t grassId;
    size_t popupId;
	
    float cellSize;

//GAMEPLAY
//========
public:
    std::shared_ptr<Workshop> addWorkshop(size_t inWorkshopId) const;

    std::shared_ptr<Workshop> addWorkshop(const std::string& inWorkshopName) const;

    std::shared_ptr<MovableTrader> addMovableTrader(size_t inWorkshopId) const;

    std::shared_ptr<MovableTrader> addMovableTrader(const std::string& inWorkshopName) const;

    Workshop* findAvailableWorkshop(size_t inJobId) const;

	[[nodiscard]] const GridManager& getGridManager() const;

private:

    void traderAddedCallback(class Trader* inTrader);
	
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
