#include "GameManager.h"

#include <memory>
#include <filesystem>
#include <fstream>

#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "NavigationSystem.h"
#include "Workshop.h"

const sf::Int32 GameManager::maxFPS = 60;

void GameManager::init(const char *prefabsPath)
{
    assert(std::filesystem::exists(prefabsPath) && std::filesystem::is_directory(prefabsPath));

    std::vector<nlohmann::json> parsedMovableTraders;
    std::vector<nlohmann::json> parsedWorkshops;

    std::ifstream fileStream;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(prefabsPath))
    {
        if (std::filesystem::is_regular_file(entry.status()) && entry.path().extension() == ".json")
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
    }
    initMovableTraders(parsedMovableTraders);
    initWorkshops(parsedWorkshops);
    initEconomicEngine(prefabsPath);
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

// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "g_windowTitle")),
                             isInitialized(false), isRunning(false), isGuiOpened(false), hasEverRun(false),
                             cameraPosition(0, 0), zoom(100)
{
    window->setFramerateLimit(maxFPS);
}


void GameManager::initEconomicEngine(const char *prefabsPath)
{
    economicEngineThread = std::make_unique<std::thread>(
            [this, prefabsPath]() -> int
            {
                auto *economicEngine = EconomicEngine::getInstance();
                economicEngine->getPostInitSignal().connect(this,
                                                            &GameManager::askResolvedCallback);
                economicEngine->getPostInitSignal().connect([this]()
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
        auto *movableTrader = new MovableTrader(parsedMovableTrader["job"]);
        movableTraderFactory.registerObject(hasher(parsedMovableTrader["job"]), movableTrader);
    }
}

void GameManager::initWorkshops(std::vector<nlohmann::json> &parsedWorkshops)
{
    const std::hash<std::string> hash;
    for (const auto &parsedWorkshop : parsedWorkshops)
    {
        auto *workshop = new Workshop(parsedWorkshop["name"], parsedWorkshop["job"]);
        workshopFactory.registerObject(hash(parsedWorkshop["name"]), workshop);
    }
}

void GameManager::processInput()
{
    sf::Event event{};
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) cameraPosition.second -= zoom / 25.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraPosition.second += zoom / 25.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) cameraPosition.first -= zoom / 25.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraPosition.first += zoom / 25.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) zoom -= 10.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) zoom += 10.f;


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
    window->clear();

    auto grid = gridManager.grid;

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(1, 1));
    rectangle.setOutlineThickness(1);

    for (int y = grid.getMinCoordinate().second; y < grid.getMaxCoordinate().second; ++y)
    {
        for (int x = grid.getMinCoordinate().first; x < grid.getMaxCoordinate().first; ++x)
        {
            rectangle.setPosition(window->getSize().x / 2.f + float(x),
                                  window->getSize().y / 2.f + float(y));
            rectangle.setOutlineColor(sf::Color::Green);
            window->draw(rectangle);
        }
    }

    for (const auto &ws : workshops)
    {
        rectangle.setPosition(window->getSize().x / 2.f + float(ws->x),
                              window->getSize().y / 2.f + float(ws->y));
        rectangle.setOutlineColor(sf::Color::Red);
        window->draw(rectangle);
    }

    sf::View view;
    view.reset(sf::FloatRect((window->getSize().x / 2.f) - zoom / 2.f + cameraPosition.first,
                             (window->getSize().y / 2.f) - zoom / 2.f + cameraPosition.second,
                             zoom, zoom));

    window->setView(view);
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
