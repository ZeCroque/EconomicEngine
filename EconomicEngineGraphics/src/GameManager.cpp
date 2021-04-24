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

    std::ifstream fileStream;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(contentPath))
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
    gridManager.init();
}

void GameManager::exec()
{
    isRunning = true;

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

bool GameManager::getHasEverRun() const
{
    return hasEverRun;
}

const sf::Texture &GameManager::getTexture(size_t textureId) const
{
    return texturesDictionary[textureId];
}

// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "g_windowTitle")),
                             caseSize(62.f),
                             background(), view(), isInitialized(false), isRunning(false), isGuiOpened(false),
                             hasEverRun(false), backgroundNeedsUpdate(true), moving(false), grassId(), oldPos()
{
    window->setFramerateLimit(maxFPS);
    view.setCenter(0, 0);
}


void GameManager::initEconomicEngine(const char *prefabsPath)
{
    economicEngineThread = std::make_unique<std::thread>(
            [this, prefabsPath]() -> int
            {
                auto *economicEngine = EconomicEngine::getInstance();
                economicEngine->getPostInitSignal().connect(this,
                                                            &GameManager::askResolvedCallback);
                economicEngine->getPostInitSignal().connect(
                        [this]()
                        {
                            isInitialized = true;
                        });

                auto *traderManager = TraderManager::getInstance();
                traderManager->getAddTraderSignal().connect(this,
                                                            &GameManager::traderAddedCallback);

                economicEngine->init(prefabsPath);

                return economicEngine->exec(100);
            });
}

void GameManager::initMovableTraders(std::vector<nlohmann::json> &parsedMovableTraders)
{
    const std::hash<std::string> hasher;
    for (const auto &parsedMovableTrader : parsedMovableTraders)
    {
        auto *movableTrader = new MovableTrader(parsedMovableTrader["job"], parsedMovableTrader["texture"]);
        movableTraderFactory.registerObject(hasher(parsedMovableTrader["job"]), movableTrader);
    }
}

void GameManager::initWorkshops(std::vector<nlohmann::json> &parsedWorkshops)
{
    const std::hash<std::string> hash;
    for (const auto &parsedWorkshop : parsedWorkshops)
    {
        auto *workshop = new Workshop(parsedWorkshop["name"], parsedWorkshop["job"], parsedWorkshop["texture"]);
        workshopFactory.registerObject(hash(parsedWorkshop["name"]), workshop);
    }
}

void GameManager::initTexture(const std::filesystem::path &path)
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
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                quit();
                break;
            }
            case sf::Event::Resized:
            {
                auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
                view.reset(sf::Rect(viewOrigin.x, viewOrigin.y, static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == 0)
                {
                    moving = true;
                    oldPos = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
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
                backgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (!moving)
                    break;

                const sf::Vector2f newPos = window->mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                sf::Vector2f deltaPos = oldPos - newPos;

                auto &grid = gridManager.grid;
                auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
                auto margin = caseSize * 10.f;

                if (viewOrigin.x + deltaPos.x + margin >= grid.getMinCoordinate().first * caseSize &&
                    viewOrigin.x + view.getSize().x + deltaPos.x - margin <= grid.getMaxCoordinate().first * caseSize)
                {
                    view.setCenter(view.getCenter().x + deltaPos.x, view.getCenter().y);
                    window->setView(view);
                }

                if (viewOrigin.y + deltaPos.y + margin >= grid.getMinCoordinate().second * caseSize &&
                    viewOrigin.y + view.getSize().y + deltaPos.y - margin <= grid.getMaxCoordinate().second * caseSize)
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
                auto &grid = gridManager.grid;
                auto newOrigin = (view.getCenter() - (view.getSize() * 1.10f) / 2.f);
                auto margin = caseSize * 5.f;

                bool canUnZoom = newOrigin.x + margin >= grid.getMinCoordinate().first * caseSize &&
                                 newOrigin.y + margin >= grid.getMinCoordinate().second * caseSize &&
                                 newOrigin.x + view.getSize().x - margin <= grid.getMaxCoordinate().first * caseSize &&
                                 newOrigin.y + view.getSize().y - margin <= grid.getMaxCoordinate().second * caseSize;

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
                }
                backgroundNeedsUpdate = true;
                break;
            }
            default:;
        }
    }

    window->setView(view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
    {
        if (!isGuiOpened)
        {
            isGuiOpened = true;

            if (!debugGuiThread)
            {
                debugGuiThread = std::make_unique<std::thread>(
                        [this]() -> int
                        {
                            int argc = 0;
                            QApplication app(argc, nullptr);

                            int result;
                            do
                            {
                                EconomicEngineDebugGui debugGui;
                                debugGui.show();
                                result = QApplication::exec();

                                isGuiOpened = false;
                                while (!isGuiOpened && isRunning);
                            } while (isRunning);

                            return result;
                        });
            }
        }
    }
}

void GameManager::update(float deltaTime)
{
    if (isInitialized)
    {
        hasEverRun = true;
        while (!pendingTraders.empty())
        {
            auto trader = std::shared_ptr<MovableTrader>(pendingTraders.front());
            pendingTraders.pop();
            traders.push_back(trader);

            auto *availableWorkshop = findAvailableWorkshop(trader->getJobId());
            if (availableWorkshop)
            {
                availableWorkshop->setTrader(trader);
            }
            else
            {
                auto workshop = addWorkshop(workshopFactory.getIdByJobId(trader->getJobId()));
                workshop->setTrader(trader);
                gridManager.queueWorkshop(workshop);
                //NavigationSystem::drawPath(gridManager.grid, std::pair(workshop->x, workshop->y), std::pair(0, 0));
            }
        }
    }
}

void GameManager::render() const
{
    auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);

    if (backgroundNeedsUpdate)
    {
        background.clear();
        background.create(view.getSize().x, view.getSize().y);
        background.setView(view);

        auto &grid = gridManager.grid;

        float viewXMin = viewOrigin.x - caseSize;
        float viewXMax = viewOrigin.x + view.getSize().x;
        float viewYMin = viewOrigin.y - caseSize;
        float viewYMax = viewOrigin.y + view.getSize().y;

        auto groundYMax = static_cast<int>(std::max(static_cast<float>(grid.getMaxCoordinate().second), viewYMax / caseSize) + 1.0f);
        auto groundXMax = static_cast<int>(std::max(static_cast<float>(grid.getMaxCoordinate().first), viewXMax / caseSize) + 1.0f);
        auto groundYMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().second), viewYMin / caseSize));
        auto groundXMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().first), viewXMin / caseSize));


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
            auto positionX = static_cast<float>(ws->x) * caseSize;
            auto positionY = static_cast<float>(ws->y) * caseSize;
            if (positionX >= viewXMin && positionX <= viewXMax && positionY >= viewYMin && positionY <= viewYMax)
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
    window->display();
}

void GameManager::quit()
{
    isRunning = false;

    EconomicEngine::getInstance()->stop();
    economicEngineThread->join();
    gridManager.getGenerationThread().join();

    if (debugGuiThread)
    {
        if (isGuiOpened)
        {
            QApplication::quit();
        }
        debugGuiThread->join();
    }

    window->close();
}

void GameManager::traderAddedCallback(Trader *trader)
{
    auto *movableTrader = movableTraderFactory.createObject(trader->getCurrentJob()->getId());  //TODO Connect to trader
    pendingTraders.push(movableTrader);
}

Workshop *GameManager::findAvailableWorkshop(size_t jobId) const
{
    for (const auto &ws : workshops)
    {
        if (ws && ws->getJobId() == jobId && ws->isAvailable())
        {
            return ws.get();
        }
    }
    return nullptr;
}

void GameManager::askResolvedCallback()
{

}

std::shared_ptr<Workshop> GameManager::addWorkshop(size_t key) const
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

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(size_t key) const
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

void GameManager::setBackgroundNeedsUpdate(bool value)
{
    backgroundNeedsUpdate = value;
}
