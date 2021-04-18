#include "GameManager.h"

#include <memory>
#include <filesystem>
#include <fstream>


#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "NavigationSystem.h"
#include "Workshop.h"

const sf::Int32 GameManager::maxFPS = 60;

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

// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "g_windowTitle")), isInitialized(false), isRunning(false), isGuiOpened(false)
{
	window->setFramerateLimit(maxFPS);
}

void GameManager::initEconomicEngine(const char* prefabsPath)
{

    	auto* economicEngine = EconomicEngine::getInstance();
	    economicEngine->init(prefabsPath);   	
	    economicEngine->start(100);
		economicEngine->getAsksResolvedSignal().connect(this, &GameManager::askResolvedCallback);

        auto* traderManager = TraderManager::getInstance();
	    traderManager->getTraderAddedSignal().connect(this, &GameManager::traderAddedCallback);
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
	const std::hash<std::string> hash;
	for(const auto& parsedWorkshop : parsedWorkshops)
	{
		auto* workshop = new Workshop(parsedWorkshop["name"],parsedWorkshop["job"]);
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
    {
        if(!isGuiOpened)
        {
        	isGuiOpened = true;

        	if(!debugGuiThread)
        	{
	            debugGuiThread = std::make_unique<std::thread>([this]() -> int 
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
            			while(!isGuiOpened && isRunning);
	                } while(isRunning);
            		
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
        EconomicEngine::getInstance()->update(deltaTime);
    	
        std::vector<std::shared_ptr<StaticActor>> workshopToPlace;

        while (!pendingTraders.empty()) 
        {
            auto trader = std::shared_ptr<MovableTrader>(pendingTraders.front());
            traders.push_back(trader);

            auto *availableWorkshop = findAvailableWorkshop(trader->getJobId());
            if (availableWorkshop) 
            {
                availableWorkshop->setTrader(trader);
            }
        	else 
            {       		
            	auto workshop = addWorkshop(workshopFactory.getIdByJobId(trader->getJobId()));
                workshops.emplace_back(workshop);
                workshopToPlace.emplace_back(workshop);
            }
            pendingTraders.pop();
        }

        if (!workshopToPlace.empty()) 
        {
            gridManager.placeWorkshop(0, 0, workshopToPlace);
            gridManager.makeDebugFile();
        	//NavigationSystem::drawPath(gridManager.grid, std::pair(workshopToPlace[0]->x, workshopToPlace[0]->y), std::pair(0,0));
        }
    }
}

void GameManager::render() const
{
    window->clear();

	//TODO rendering
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

void GameManager::quit()
{
	isRunning = false;

	
    if(debugGuiThread)
    {
    	if(isGuiOpened)
    	{
    		QApplication::quit();
    	}
	    debugGuiThread->join();
    }
	
    window->close();
}

void GameManager::traderAddedCallback(Trader* trader)
{
    auto* movableTrader = movableTraderFactory.createObject(trader->getCurrentJob()->getId());  //TODO Connect to trader
    pendingTraders.push(movableTrader);
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

std::shared_ptr<Workshop> GameManager::addWorkshop(const std::string& name) const
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

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const std::string& name) const
{
	const std::hash<std::string> hash;
	return addMovableTrader(hash(name));
}

Workshop* GameManager::findAvailableWorkshop(size_t jobId) const
{
    for (const auto &ws : workshops) 
    {
        if (ws->getJobId() == jobId && ws->isAvailable()) 
        {
            return ws.get();
        }
    }
    return nullptr;
}
 
