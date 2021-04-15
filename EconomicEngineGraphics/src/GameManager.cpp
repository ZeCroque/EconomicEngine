#include "GameManager.h"

#include <memory>
#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "Workshop.h"

const sf::Int32 GameManager::maxFPS = 60;

GameManager::~GameManager() = default;

void GameManager::init(const char* prefabsPath)
{
		assert(std::filesystem::exists(prefabsPath) && std::filesystem::is_directory(prefabsPath));

		std::vector<nlohmann::json> parsedMovableTraders;
		std::vector<nlohmann::json> parsedWorkshops;
		
		std::ifstream fileStream;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(prefabsPath))
		{
			if (std::filesystem::is_regular_file(entry.status()) && entry.path().extension() == ".json")
			{
				nlohmann::json parsedJson;
				
				fileStream.open (entry.path());
				fileStream >> parsedJson;
			    fileStream.close();

				if(parsedJson["type"] == "MovableTrader")
				{
					parsedMovableTraders.push_back(parsedJson);
				}
				else if(parsedJson["type"] == "Workshop")
				{
					parsedWorkshops.push_back(parsedJson);
				}
			}
		}
		initMovableTraders(parsedMovableTraders);
		initWorkshops(parsedWorkshops);
		initThreads(prefabsPath);
}

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

        processInput();

        while (lag >= deltaTimeUs) {
            update(deltaTimeS);
            lag -= deltaTimeUs;
        }

        render();
    }
}

const WorkshopFactory& GameManager::getWorkshopFactory() const
{
	return workshopFactory;
}

const MovableTraderFactory& GameManager::getMovableTraderFactory() const
{
	return movableTraderFactory;
}
// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "g_windowTitle")),
                             economicEngineInitialized(false), isGuiOpened(false) {
	window->setFramerateLimit(maxFPS);
}

void GameManager::initThreads(const char* prefabsPath)
{
	auto *turnManager = EconomicEngine::getInstance();
    turnManager->addObserver(this);
    turnManager->getPostInitSignal().connect([this](const std::any &) {
        economicEngineInitialized = true;
    });

    economicEngineThread = std::make_unique<std::thread>([prefabsPath, turnManager]() -> int {
        turnManager->init(prefabsPath);
        return turnManager->exec(100);
    });

	
    auto *traderManager = TraderManager::getInstance();
    traderManager->getAddTraderSignal().connect([this](Trader *trader) {
        auto *movableTrader = movableTraderFactory.createObject(trader->getCurrentJob()->getId());  //TODO: Connect
        pendingTraders.push(movableTrader);
    });
}

void GameManager::initMovableTraders(std::vector<nlohmann::json>& parsedMovableTraders)
{
	const std::hash<std::string> hasher;
	for(const auto& parsedMovableTrader : parsedMovableTraders)
	{
		auto hash = parsedMovableTrader["job"];
		auto* movableTrader = new MovableTrader(parsedMovableTrader["job"]);
		movableTraderFactory.registerObject(hasher(parsedMovableTrader["job"]), movableTrader);
	}
}

void GameManager::initWorkshops(std::vector<nlohmann::json>& parsedWorkshops)
{
	const std::hash<std::string> hasher;
	for(const auto& parsedWorkshop : parsedWorkshops)
	{
		auto hash = parsedWorkshop["job"];
		auto* workshop = new Workshop(parsedWorkshop["job"]);
		workshopFactory.registerObject(hasher(parsedWorkshop["job"]), workshop);
	}
}

void GameManager::processInput() {
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

void GameManager::update(float deltaTime) {

    if (economicEngineInitialized) {

        std::vector<std::shared_ptr<StaticActor>> workshopToPlace;

        while (!pendingTraders.empty()) {
            auto trader = std::shared_ptr<MovableTrader>(pendingTraders.front());
            traders.push_back(trader);

            auto *availableWorkshop = findAvailableWorkshop(trader->getJobId());
            if (availableWorkshop) {
                availableWorkshop->setTrader(trader);
            } else {
            	auto workshop = std::shared_ptr<Workshop>(workshopFactory.createObject(trader->getJobId()));
                workshop->setTrader(trader);
                workshops.emplace_back(workshop);
                workshopToPlace.emplace_back(workshop);
            }
            pendingTraders.pop();
        }

        if (!workshopToPlace.empty()) {
            gridManager.placeWorkshop(0, 0, workshopToPlace);
            gridManager.makeDebugFile();
        }
    }
}

void GameManager::render() const
{
    window->clear();

    sf::CircleShape shape(100.f);

    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<> distrib(1, 100);

    const sf::Uint8 greyValue = distrib(gen);
    shape.setFillColor(sf::Color(greyValue, greyValue, greyValue));
    shape.setPosition(100, 100);

    window->draw(shape);
    window->display();
}

void GameManager::quit() const
{
    EconomicEngine::getInstance()->stop();
    economicEngineThread->join();

    window->close();
}

void GameManager::guiCloseSignalCallback(std::any lhs) {
    isGuiOpened = false;
}

void GameManager::notify(Observable *sender) {
    //TODO replace signal slot
}
 
