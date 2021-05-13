#include "GameManager.h"

#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/adl_serializer.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <nlohmann/adl_serializer.hpp>

#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "NavigationSystem.h"
#include "Workshop.h"

const sf::Int32 GameManager::maxFPS = 60;

void GameManager::init(const char *inContentPath)
{
    assert(std::filesystem::exists(inContentPath) && std::filesystem::is_directory(inContentPath));

    std::vector<nlohmann::json> parsedMovableTraders;
    std::vector<nlohmann::json> parsedWorkshops;

    std::ifstream fileStream;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(inContentPath))
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
    initEconomicEngine(inContentPath);
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
        const auto currentTimestamp = clock.getElapsedTime().asMicroseconds();
        const auto timeSinceLastFrame = currentTimestamp - previousTimestamp;
        previousTimestamp = currentTimestamp;
        lag += timeSinceLastFrame;

        processInput();

        while (lag >= deltaTimeUs)
        {
            update(deltaTimeS);
            lag -= deltaTimeUs;
        }

        render();
    }
}

bool GameManager::getIsRunning() const
{
    return isRunning;
}

const sf::Texture &GameManager::getTexture(const size_t inTextureId) const
{
    return texturesDictionary[inTextureId];
}

bool GameManager::getIsInitialized() const
{
    return isInitialized;
}

// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : speedFactor(1.f),
                             window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "g_windowTitle")),
                             moving(false), drawPopup(false),
                             isInitialized(false), isRunning(false), isGuiOpened(false), wantsToOpenGui(false),
                             backgroundNeedsUpdate(true), selectedActor(nullptr),grassId(0), caseSize(62.f)
{
    window->setFramerateLimit(maxFPS);
    view.setCenter(0, 0);
}


void GameManager::initEconomicEngine(const char *inPrefabsPath)
{
    auto *economicEngine = EconomicEngine::getInstance();
    economicEngine->init(inPrefabsPath);
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

void GameManager::initMovableTraders(std::vector<nlohmann::json> &inParsedMovableTraders)
{
    const std::hash<std::string> hasher;
    for (const auto &parsedMovableTrader : inParsedMovableTraders)
    {
        auto *movableTrader = new MovableTrader(parsedMovableTrader["job"], parsedMovableTrader["texture"]);
        movableTraderFactory.registerObject(hasher(parsedMovableTrader["job"]), movableTrader);
    }
}

void GameManager::initWorkshops(std::vector<nlohmann::json> &inParsedWorkshops)
{
    const std::hash<std::string> hash;
    for (const auto &parsedWorkshop : inParsedWorkshops)
    {
        auto *workshop = new Workshop(parsedWorkshop["name"], parsedWorkshop["job"], parsedWorkshop["texture"]);
        workshopFactory.registerObject(hash(parsedWorkshop["name"]), workshop);
    }
}

void GameManager::initTexture(const std::filesystem::path &inPath) const
{
    sf::Texture texture;
    texture.loadFromFile(inPath.string());

    const std::hash<std::string> hash;
    texturesDictionary.emplace(hash(inPath.filename().string()), texture);
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

void GameManager::update(const float inDeltaTime)
{
    EconomicEngine::getInstance()->update(inDeltaTime);

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

    for (auto &trader : traders)
    {
        if (trader->direction != Direction::None)
        {
            trader->coordinatesOffset += caseSize * inDeltaTime;
        }
    }
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

void GameManager::setSpeedFactor(const float inSpeedFactor)
{
    speedFactor = inSpeedFactor;
}

float GameManager::getSpeedFactor() const
{
    return speedFactor;
}

void GameManager::traderAddedCallback(Trader *inTrader)
{
    auto *movableTrader = movableTraderFactory.createObject(inTrader->getCurrentJob()->getId());
    pendingTraders.push(movableTrader);
    inTrader->getMoveToRequestSignal().connect([inTrader, movableTrader](Position position)
                                             {
                                                 movableTrader->moveToRequestCallback(inTrader, position);
                                             });

    inTrader->getDeathSignal().connect([movableTrader, this]()
                                     {
                                         movableTrader->getPathfindEndedSignal().disconnectAll();
                                         traders.remove_if(
                                                 [movableTrader](const std::shared_ptr<MovableTrader> &inMovableTrader)
                                                 {
                                                     return inMovableTrader.get() == movableTrader;
                                                 });
                                     });
}

Workshop *GameManager::findAvailableWorkshop(const size_t inJobId) const
{
    const auto result = std::ranges::find_if(workshops.begin(), workshops.end(),
                                             [inJobId](const std::shared_ptr<Workshop> &ws)
                                             {
                                                 return ws->getJobId() == inJobId && ws->isAvailable();
                                             });

    return result == workshops.end() ? nullptr : result->get();
}

const GridManager &GameManager::getGridManager() const
{
    return gridManager;
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const size_t inKey) const
{
    auto workshop = std::shared_ptr<Workshop>(workshopFactory.createObject(inKey));
    workshops.push_back(workshop);
    return workshop;
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const std::string &inName) const
{
    const std::hash<std::string> hash;
    return addWorkshop(hash(inName));
}

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const size_t inKey) const
{
    auto movableTrader = std::shared_ptr<MovableTrader>(movableTraderFactory.createObject(inKey));
    traders.push_back(movableTrader);
    return movableTrader;
}

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const std::string &inName) const
{
    const std::hash<std::string> hash;
    return addMovableTrader(hash(inName));
}

void GameManager::setBackgroundNeedsUpdate(const bool inValue) const
{
    backgroundNeedsUpdate = inValue;
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
