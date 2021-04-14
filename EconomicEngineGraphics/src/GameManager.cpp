#include "GameManager.h"

#include <iostream>
#include <memory>
#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "Workshop.h"

const sf::Int32 GameManager::maxFPS = 60;

GameManager::~GameManager() = default;

void GameManager::exec() {
    const sf::Clock clock;
    auto previousTimestamp = clock.getElapsedTime().asMicroseconds();
    sf::Int64 lag = 0;

    const auto deltaTime = sf::seconds(1.f / maxFPS);
    const auto deltaTimeUs = deltaTime.asMicroseconds();
    const auto deltaTimeS = deltaTime.asSeconds();

    while (window->isOpen()) {
        const auto currentTimestamp = clock.getElapsedTime().asMicroseconds();
        const auto timeSinceLastFrame = currentTimestamp - previousTimestamp;
        previousTimestamp = currentTimestamp;
        lag += timeSinceLastFrame;

        ProcessInput();

        while (lag >= deltaTimeUs) {
            Update(deltaTimeS);
            lag -= deltaTimeUs;
        }

        Render();
    }
}

// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "g_windowTitle")),
                             economicEngineInitialized(false), isGuiOpened(false) {
    window->setFramerateLimit(maxFPS);
    auto *turnManager = EconomicEngine::getInstance();
    turnManager->addObserver(this);
    turnManager->getPostInitSignal().connect([this](const std::any &) {
        economicEngineInitialized = true;
    });

    auto *traderManager = TraderManager::getInstance();
    traderManager->getAddTraderSignal().connect([this](Trader *trader) {
        auto *movableTrader = new MovableTrader();
        movableTrader->setJobId(trader->getCurrentJob()->getId());
        pendingTrader.push(movableTrader); //TODO: Connect
    });


    economicEngineThread = std::make_unique<std::thread>([](EconomicEngine *turnManager) -> int {
        turnManager->init("./Content/Prefabs/");
        return turnManager->exec(100);
    }, turnManager);
}

void GameManager::ProcessInput() {
    sf::Event event{};
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            quit();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (!isGuiOpened) {
            isGuiOpened = true;
            debugGUIThread = std::make_unique<std::thread>([this]() -> int {
                int argc = 0;
                QApplication a(argc, nullptr);

                EconomicEngineDebugGui w;
                w.getCloseSignal().connect(this, &GameManager::guiCloseSignalCallback);
                w.show();
                return QApplication::exec();
            });
            debugGUIThread->detach();
        }
    }
}

Workshop *GameManager::findAvailableWorkshop(size_t jobId) {
    for (const auto &ws : workshops) {
        if (ws->getJobId() == jobId && ws->isAvailable()) {
            return ws.get();
        }
    }
    return nullptr;
}

void GameManager::Update(float deltaTime) {

    if (economicEngineInitialized) {

        std::vector<std::shared_ptr<StaticActor>> workshopToPlace;

        while (!pendingTrader.empty()) {
            auto trader = std::shared_ptr<MovableTrader>(pendingTrader.front());
            traders.push_back(trader);

            auto *availableWorkshop = findAvailableWorkshop(trader->getJobId());
            if (availableWorkshop) {
                availableWorkshop->setTrader(trader);
            } else {
                auto *newWorkshop = new Workshop();
                newWorkshop->setTrader(trader);
                newWorkshop->setJobId(trader->getJobId());

                workshops.emplace_back(newWorkshop);
                workshopToPlace.emplace_back(newWorkshop);
            }
            std::cout << workshopToPlace[0].use_count() << std::endl;
            pendingTrader.pop();
        }

        if (!workshopToPlace.empty()) {
            gridManager.placeWorkshop(0, 0, workshopToPlace);
        }

    }
}

void GameManager::Render() {
    window->clear();

    sf::CircleShape shape(100.f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);

    auto greyValue = distrib(gen);
    shape.setFillColor(sf::Color(greyValue, greyValue, greyValue));
    shape.setPosition(100, 100);

    window->draw(shape);
    window->display();
}

void GameManager::quit() {
    EconomicEngine::getInstance()->stop();
    economicEngineThread->join();

    window->close();
}

void GameManager::guiCloseSignalCallback(std::any lhs) {
    std::cout << "wsh" << std::endl;
    isGuiOpened = false;
}

void GameManager::notify(Observable *sender) {
    //TODO handle economic engine
}
 
