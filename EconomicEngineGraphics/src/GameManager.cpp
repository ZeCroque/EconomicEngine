#include "GameManager.h"

#include <memory>
#include <filesystem>
#include <fstream>
#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "NavigationSystem.h"
#include "Workshop.h"

const sf::Int32 GameManager::maxFPS = 60;

void GameManager::init(const char *contentPath)
{
    assert(std::filesystem::exists(contentPath) && std::filesystem::is_directory(contentPath));

    std::vector<nlohmann::json> parsedMovableTraders;
    std::vector<nlohmann::json> parsedWorkshops;

    for (std::ifstream fileStream; const auto &entry : std::filesystem::recursive_directory_iterator(contentPath))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            if (entry.path().extension() == ".json")
            {
                nlohmann::json parsedJson;

                fileStream.open(entry.path());
                fileStream >> parsedJson;
                fileStream.close();

                if (parsedJson["type"] == "MovableTrader")
                {
                    parsedMovableTraders.push_back(parsedJson);
                }
                else if (parsedJson["type"] == "Workshop")
                {
                    parsedWorkshops.push_back(parsedJson);
                }
            }
            else if (entry.path().extension() == ".png")
            {
                initTexture(entry.path());
            }
        }
    }

    const std::hash<std::string> hash;
    grassId = hash("TX_Grass.png");

    initMovableTraders(parsedMovableTraders);
    initWorkshops(parsedWorkshops);
    initEconomicEngine(contentPath);
    initGui();
    gridManager.init();
}

void GameManager::exec()
{
    // ReSharper disable once CppPossiblyErroneousEmptyStatements
    while (!isInitialized); // NOLINT(clang-diagnostic-empty-body)

    isRunning = true;

    EconomicEngine::getInstance()->start(50);

    const sf::Clock clock;
    auto previousTimestamp = clock.getElapsedTime().asMicroseconds();
    sf::Int64 lag = 0;

    const auto deltaTime = sf::seconds(1.f / maxFPS);
    const auto deltaTimeUs = deltaTime.asMicroseconds();
    const auto deltaTimeS = deltaTime.asSeconds();

    while (window->isOpen())
    {
	    if(const auto currentTimestamp = clock.getElapsedTime().asMicroseconds(); !isPaused)
		{
        	const auto timeSinceLastFrame = currentTimestamp - previousTimestamp;
	        previousTimestamp = currentTimestamp;
	        lag += timeSinceLastFrame;

	        processInput();

	        while (lag >= deltaTimeUs)
	        {
				update(deltaTimeS * speedFactor);
	            lag -= deltaTimeUs;
	        }
        }
        else
        {
	        previousTimestamp = currentTimestamp;
        }
    	render();
    }
}

void GameManager::resume()
{	
	isPaused = false;
}

void GameManager::reset(const int inTradersCount)
{
	auto& grid = gridManager.getGrid();
    const auto minCoords = grid.getMinCoordinate();
	const auto maxCoords = grid.getMaxCoordinate();
	
	gridManager.reset();
	
	while(!pendingTraders.empty())
	{
		auto* pendingTrader = pendingTraders.front();
		delete pendingTrader;
		pendingTraders.pop();
	}

	traders.clear();
	workshops.clear();
	
	gridManager.init();
    EconomicEngine::getInstance()->reset(inTradersCount);

	drawPopup = false;
	backgroundNeedsUpdate = true;
}

void GameManager::pause()
{
	isPaused = true;
}

bool GameManager::getIsRunning() const
{
    return isRunning;
}

bool GameManager::getIsInitialized() const
{
    return isInitialized;
}

void GameManager::setBackgroundNeedsUpdate(const bool value) const
{
    backgroundNeedsUpdate = value;
}

void GameManager::setSpeedFactor(const float inSpeedFactor)
{
    speedFactor = inSpeedFactor;
}

float GameManager::getSpeedFactor() const
{
    return speedFactor;
}

const sf::Texture &GameManager::getTexture(const size_t textureId) const
{
    return texturesDictionary[textureId];
}

// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "g_windowTitle")),
                             moving(false), drawPopup(false),
                             isInitialized(false), isRunning(false), isPaused(false), isGuiOpened(false), wantsToOpenGui(false), speedFactor(1.f),
                             backgroundNeedsUpdate(true), selectedActor(nullptr), grassId(0), caseSize(62.f)
{
    window->setFramerateLimit(maxFPS);
    view.setCenter(0, 0);
}


void GameManager::initEconomicEngine(const char *prefabsPath)
{
    auto *economicEngine = EconomicEngine::getInstance();
    economicEngine->init(prefabsPath);
    economicEngine->getTraderManager().getTraderAddedSignal().connect(this, &GameManager::traderAddedCallback);
}

void GameManager::initGui()
{
    debugGuiThread = std::make_unique<std::thread>([this]() -> int
                                                   {
                                                       int argc = 0;
                                                       QApplication app(argc, nullptr);

                                                       int result;
                                                       do
                                                       {
                                                           EconomicEngineDebugGui debugGui;
                                                           debugGui.getInitializedSignal().connect([this]()
                                                                                                   {
                                                                                                       isInitialized = true;
                                                                                                   });
                                                           debugGui.show();
                                                           isGuiOpened = true;
                                                           wantsToOpenGui = false;
                                                           result = QApplication::exec();

                                                           isGuiOpened = false;
                                                           while (!wantsToOpenGui && isRunning);
                                                       } while (isRunning);

                                                       return result;
                                                   });
}

void GameManager::initMovableTraders(std::vector<nlohmann::json> &parsedMovableTraders)
{
    for (const std::hash<std::string> hasher; const auto &parsedMovableTrader : parsedMovableTraders)
    {
        auto *movableTrader = new MovableTrader(parsedMovableTrader["job"], parsedMovableTrader["texture"]);
        movableTraderFactory.registerObject(hasher(parsedMovableTrader["job"]), movableTrader);
    }
}

void GameManager::initWorkshops(std::vector<nlohmann::json> &parsedWorkshops)
{
    for (const std::hash<std::string> hash; const auto &parsedWorkshop : parsedWorkshops)
    {
        auto *workshop = new Workshop(parsedWorkshop["name"], parsedWorkshop["job"], parsedWorkshop["texture"]);
        workshopFactory.registerObject(hash(parsedWorkshop["name"]), workshop);
    }
}

void GameManager::initTexture(const std::filesystem::path &path) const
{
    sf::Texture texture;
    texture.loadFromFile(path.string());

    const std::hash<std::string> hash;
    texturesDictionary.emplace(hash(path.filename().string()), texture);
}

void GameManager::processInput()
{
    sf::Event event{};

    while (window->pollEvent(event))
    {
        switch (event.type)  // NOLINT(clang-diagnostic-switch-enum)
        {
            case sf::Event::Closed:
            {
                quit();
                break;
            }
            case sf::Event::Resized:
            {
                const auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
                view.reset(sf::Rect<float>(viewOrigin.x, viewOrigin.y, static_cast<float>(event.size.width),
                                           static_cast<float>(event.size.height)));
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == 0)
                {
                    moving = true;
                    oldPos = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    clickPos = oldPos;
                }
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == 0)
                {
                    moving = false;
                }
                if (oldPos == clickPos)
                {
                    getSelectedActor();
                }
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (!moving)
                    break;

                const sf::Vector2f newPos = window->mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                const sf::Vector2f deltaPos = oldPos - newPos;

                const auto &grid = gridManager.getGrid();
                const auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
                const auto margin = caseSize * 10.f;

                if (viewOrigin.x + deltaPos.x + margin >=
                    static_cast<float>(grid.getMinCoordinate().first) * caseSize &&
                    viewOrigin.x + view.getSize().x + deltaPos.x - margin <=
                    static_cast<float>(grid.getMaxCoordinate().first) * caseSize)
                {
                    view.setCenter(view.getCenter().x + deltaPos.x, view.getCenter().y);
                    window->setView(view);
                }

                if (viewOrigin.y + deltaPos.y + margin >=
                    static_cast<float>(grid.getMinCoordinate().second) * caseSize &&
                    viewOrigin.y + view.getSize().y + deltaPos.y - margin <=
                    static_cast<float>(grid.getMaxCoordinate().second) * caseSize)
                {
                    view.setCenter(view.getCenter().x, view.getCenter().y + deltaPos.y);
                    window->setView(view);
                }

                oldPos = window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                const auto &grid = gridManager.getGrid();
                const auto newOrigin = (view.getCenter() - (view.getSize() * 1.10f) / 2.f);
                const auto margin = caseSize * 5.f;

                // ReSharper disable once CppTooWideScope
                const bool canUnZoom =
                        newOrigin.x + margin >= static_cast<float>(grid.getMinCoordinate().first) * caseSize &&
                        newOrigin.y + margin >= static_cast<float>(grid.getMinCoordinate().second) * caseSize &&
                        newOrigin.x + view.getSize().x - margin <=
                        static_cast<float>(grid.getMaxCoordinate().first) * caseSize &&
                        newOrigin.y + view.getSize().y - margin <=
                        static_cast<float>(grid.getMaxCoordinate().second) * caseSize;

                if (event.mouseWheelScroll.delta <= -1 && canUnZoom &&
                    (view.getSize().x * 1.10f < 32768.f && view.getSize().y * 1.10f < 32768.f))
                {
                    view.zoom(1.10f);
                }
                else if (event.mouseWheelScroll.delta >= 1 &&
                         (view.getSize().x * 0.90f > 400.f && view.getSize().y * 0.90f > 400.f))
                {
                    view.zoom(0.90f);
                }
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code == 57) //Space
                {
                    view.setCenter(0, 0);
                    backgroundNeedsUpdate = true;
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    if (!isGuiOpened)
                    {
                        wantsToOpenGui = true;
                    }
                }
                break;
            }
            default:;
        }
    }

    window->setView(view);
}

void GameManager::update(const float deltaTime)
{
	    EconomicEngine::getInstance()->update(deltaTime);

		updateWorkshops(deltaTime);

		updateTraders(deltaTime);
}

void GameManager::render() const
{
    auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
    float viewXMin = viewOrigin.x - caseSize;
    float viewXMax = viewOrigin.x + view.getSize().x;
    float viewYMin = viewOrigin.y - caseSize;
    float viewYMax = viewOrigin.y + view.getSize().y;

    if (backgroundNeedsUpdate)
    {
        background.clear();
        background.create(static_cast<unsigned int>(view.getSize().x), static_cast<unsigned int>(view.getSize().y));
        background.setView(view);

        const auto &grid = gridManager.getGrid();

        auto groundYMax = static_cast<int>(
                std::max(static_cast<float>(grid.getMaxCoordinate().second), viewYMax / caseSize) + 1.0f);
        auto groundXMax = static_cast<int>(
                std::max(static_cast<float>(grid.getMaxCoordinate().first), viewXMax / caseSize) + 1.0f);
        auto groundYMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().second),
                                                    viewYMin / caseSize));
        auto groundXMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().first),
                                                    viewXMin / caseSize));


        sf::Sprite groundSprite;
        groundSprite.setTexture(getTexture(grassId));

        for (int y = groundYMin; y < groundYMax; ++y)
        {
            for (int x = groundXMin; x < groundXMax; ++x)
            {
                groundSprite.setPosition(static_cast<float>(x) * caseSize, static_cast<float>(y) * caseSize);
                background.draw(groundSprite);
            }
        }

        for (auto &ws : workshops)
        {
            if (auto positionX = static_cast<float>(ws->x) * caseSize, positionY = static_cast<float>(ws->y) * caseSize; positionX >= viewXMin && positionX <= viewXMax && positionY >= viewYMin && positionY <= viewYMax)
            {
                auto &workshopSprite = ws->getSprite();
                workshopSprite.setPosition(positionX, positionY);
                background.draw(workshopSprite);
            }
        }

        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(caseSize, caseSize));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setPosition(viewXMin + caseSize, viewYMin + caseSize);
        background.draw(rectangle);
        rectangle.setPosition(viewXMax - caseSize, viewYMax - caseSize);
        background.draw(rectangle);

        background.display();

        backgroundNeedsUpdate = false;
    }


    const sf::Texture &texture = background.getTexture();
    sf::Sprite backgroundSprite(texture);
    backgroundSprite.setPosition(viewOrigin);

    window->clear();
    window->draw(backgroundSprite);

	if(!isPaused)
	{
	    for (auto &trader : traders)
	    {
	        auto &traderSprite = trader->getSprite();
	        traderSprite.setScale(1.f, 1.f);
	        traderSprite.setOrigin(16.f, 16.f);

	        if (auto x = static_cast<float>(trader->x) * caseSize, y = static_cast<float>(trader->y) * caseSize; x >= viewXMin && x <= viewXMax && y >= viewYMin && y <= viewYMax)
	        {
		        int rectLeft = 0;
		        switch (trader->direction) //NOLINT(clang-diagnostic-switch-enum)
	            {
	                case Direction::Top:
	                    y -= trader->coordinatesOffset;
	                    rectLeft = 64;
	                    break;
	                case Direction::Bottom:
	                    y += trader->coordinatesOffset;
	                    rectLeft = 0;
	                    break;
	                case Direction::Left:
	                    x -= trader->coordinatesOffset;
	                    rectLeft = 32;
	                    break;
	                case Direction::Right:
	                    x += trader->coordinatesOffset;
	                    traderSprite.setScale(-1.f, 1.f);
	                    rectLeft = 32;
	                    break;
	                default:;
	            }

	            sf::IntRect rectSourceSprite(rectLeft, 0, 32, 32);
	            traderSprite.setTextureRect(rectSourceSprite);
	            traderSprite.setPosition(x + 32.f, y + 48.f);
	            window->draw(traderSprite);
	        }

	        if (trader->coordinatesOffset >= caseSize)
	        {
	            trader->updatePath();
	        }

	    }
    }

    if (drawPopup)
    {
        auto marketPos = selectedActor->getClosestMarketCoordinate();

        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(caseSize, caseSize));
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineThickness(4);
        rectangle.setOutlineColor(sf::Color::Red);
        rectangle.setPosition(static_cast<float>(marketPos.first) * caseSize,
                              static_cast<float>(marketPos.second) * caseSize);

        window->draw(rectangle);

        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setPosition(static_cast<float>(selectedActor->x) * caseSize,
                              static_cast<float>(selectedActor->y) * caseSize);

        window->draw(rectangle);

        auto trader = selectedActor->getTrader();
        auto x = static_cast<float>(trader->x) * caseSize;
        auto y = static_cast<float>(trader->y) * caseSize;
        switch (trader->direction) //NOLINT(clang-diagnostic-switch-enum)
        {
            case Direction::Top:
                y -= trader->coordinatesOffset;
                break;
            case Direction::Bottom:
                y += trader->coordinatesOffset;
                break;
            case Direction::Left:
                x -= trader->coordinatesOffset;
                break;
            case Direction::Right:
                x += trader->coordinatesOffset;
                break;
            default:;
        }


        rectangle.setSize(sf::Vector2f(32, 32));
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color::Blue);
        rectangle.setPosition(x + 16,y + 32);

        window->draw(rectangle);
    }
    window->display();
}

void GameManager::quit()
{
    isRunning = false;

    EconomicEngine::getInstance()->getStockExchange().getAskResolvedSignal().disconnectAll();

    if (debugGuiThread)
    {
        if (isGuiOpened)
        {
            QApplication::quit();
        }
        debugGuiThread->join();
    }

    gridManager.getGenerationThread().join();
    window->close();
}

void GameManager::traderAddedCallback(Trader *trader)
{
    auto *movableTrader = movableTraderFactory.createObject(trader->getCurrentJob()->getId());
    pendingTraders.push(movableTrader);
    trader->getMoveToRequestSignal().connect([trader, movableTrader](Position position)
                                             {
                                                 movableTrader->moveToRequestCallback(trader, position);
                                             });

    trader->getDeathSignal().connect([movableTrader, this]()
                                     {
                                         movableTrader->getPathfindEndedSignal().disconnectAll();
                                         traders.remove_if(
                                                 [movableTrader](const std::shared_ptr<MovableTrader> &inMovableTrader)
                                                 {
                                                     return inMovableTrader.get() == movableTrader;
                                                 });
                                     });
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const size_t key) const
{
    auto workshop = std::shared_ptr<Workshop>(workshopFactory.createObject(key));
    workshops.push_back(workshop);
    return workshop;
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const std::string &name) const
{
    const std::hash<std::string> hash;
    return addWorkshop(hash(name));
}

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const size_t key) const
{
    auto movableTrader = std::shared_ptr<MovableTrader>(movableTraderFactory.createObject(key));
    traders.push_back(movableTrader);
    return movableTrader;
}

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const std::string &name) const
{
    const std::hash<std::string> hash;
    return addMovableTrader(hash(name));
}

Workshop *GameManager::findAvailableWorkshop(const size_t jobId) const
{
    const auto result = std::ranges::find_if(workshops.begin(), workshops.end(),
                                             [jobId](const std::shared_ptr<Workshop> &ws)
                                             {
                                                 return ws->getJobId() == jobId && ws->isAvailable();
                                             });

    return result == workshops.end() ? nullptr : result->get();
}

void GameManager::getSelectedActor()
{
    auto grid = gridManager.getGrid();

    auto x = static_cast<int>(clickPos.x / caseSize);
    auto y = static_cast<int>(clickPos.y / caseSize);

    if (clickPos.x < 0 && clickPos.y > 0)
    {
        --x;
    }
    else if ((clickPos.x < 0 && clickPos.y < 0))
    {
        --x;
        --y;
    }
    else if ((clickPos.x > 0 && clickPos.y < 0))
    {
        --y;
    }

    if (const auto actor = grid.getActorAt(x, y); actor && actor->getTrader())
    {
        selectedActor = actor;
        drawPopup = true;
    }
    else
    {
        drawPopup = false;
    }

}

const GridManager &GameManager::getGridManager() const
{
    return gridManager;
}

void GameManager::updateWorkshops(const float deltaTime)
{
	while (!pendingTraders.empty())
    {
        auto trader = std::shared_ptr<MovableTrader>(pendingTraders.front());
        pendingTraders.pop();
        traders.push_back(trader);

        if (auto *availableWorkshop = findAvailableWorkshop(trader->getJobId()); availableWorkshop)
        {
            availableWorkshop->setTrader(trader);
        }
        else
        {
            auto workshop = addWorkshop(workshopFactory.getIdByJobId(trader->getJobId()));
            workshop->setTrader(trader);
            gridManager.queueWorkshop(workshop);
        }
    }
}

void GameManager::updateTraders(const float deltaTime) const
{
	for (auto &trader : traders)
    {
        if (trader->direction != Direction::None)
        {
            trader->coordinatesOffset += caseSize * deltaTime;
        }
    }
}


