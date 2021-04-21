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
                             isInitialized(false), isRunning(false), isGuiOpened(false), hasEverRun(false),
                             zoom(1000.f), cameraPosition(0, 0)
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
        if (event.type == sf::Event::Closed)
        {
            quit();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) cameraPosition.second -= zoom * 0.10f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraPosition.second += zoom * 0.10f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) cameraPosition.first -= zoom * 0.10f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraPosition.first += zoom * 0.10f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) zoom = std::max(620.f, zoom - zoom * 0.10f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) zoom = std::min(62000.f, zoom + zoom * 0.10f);

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

    auto viewLeft = (window->getSize().x / 2.f) - zoom / 2.f + cameraPosition.first;
    auto viewTop = (window->getSize().y / 2.f) - zoom / 2.f + cameraPosition.second;

    auto &grid = gridManager.grid;


    float xMin = viewLeft - 62.f;
    float xMax = (window->getSize().x / 2.f) + zoom / 2.f + cameraPosition.first + 62;
    float yMin = viewTop - 62.f;
    float yMax = (window->getSize().y / 2.f) + zoom / 2.f + cameraPosition.second + 62;

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(62, 62));
    rectangle.setFillColor(sf::Color::Green);
    for (int y = grid.getMinCoordinate().second; y < grid.getMaxCoordinate().second + 1; ++y)
    {
        for (int x = grid.getMinCoordinate().first; x < grid.getMaxCoordinate().first + 1; ++x)
        {
            auto positionX = window->getSize().x / 2.f + float(x) * 62;
            auto positionY = window->getSize().y / 2.f + float(y) * 62;
            if (positionX >= xMin && positionX <= xMax && positionY >= yMin && positionY <= yMax)
            {
                rectangle.setPosition(positionX, positionY);
                window->draw(rectangle);
            }
        }
    }


    for (auto &ws : workshops)
    {
        auto positionX = window->getSize().x / 2.f + float(ws->x) * 62;
        auto positionY = window->getSize().y / 2.f + float(ws->y) * 62;
        if (positionX >= xMin && positionX <= xMax && positionY >= yMin && positionY <= yMax)
        {
            auto &sprite = ws->getSprite();
            sprite.setPosition(positionX, positionY);
            window->draw(sprite);
        }
    }

    window->display();

    sf::View view;
    view.reset(sf::FloatRect(xMin, yMin, xMax - xMin, yMax - yMin));
    window->setView(view);
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
