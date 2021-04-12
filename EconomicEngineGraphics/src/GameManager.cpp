#include "GameManager.h"
#include "EconomicEngineDebugGUI.h"
#include "GridManager.h"

const sf::Int32 GameManager::maxFPS = 60;

GameManager::~GameManager()
{
	if(debugGUI)
	{
		delete *debugGUI;
		delete debugGUI;
	}
}

void GameManager::Exec()
{	
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

        ProcessInput();

        while (lag >= deltaTimeUs)
        {
            Update(deltaTimeS);
            lag -= deltaTimeUs;
        }

        Render();  	
    }
}
// window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800,600), "g_windowTitle")), debugGUI(nullptr)
{
	window->setFramerateLimit(maxFPS);
	auto* turnManager = EconomicEngine::getInstance();
	turnManager->addObserver(this);
	economicEngineThread.reset(new std::thread([](EconomicEngine* turnManager)-> int
	{
		turnManager->init("./Content/Prefabs/");
		return turnManager->exec(100);
	}, turnManager));
}

void GameManager::ProcessInput()
{
	sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
			quit();
        }
    }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if(!debugGUIThread)
		{
			debugGUI = new EconomicEngineDebugGui*();
			debugGUIThread.reset(new std::thread([](EconomicEngineDebugGui** debugGui)-> int
			{
				int argc = 0;
				QApplication a(argc, nullptr);
				
				*debugGui = new EconomicEngineDebugGui();
				(*debugGui)->show();
				return QApplication::exec();
			}, debugGUI));
        }
	}
}

void GameManager::Update(float deltaTime)
{

}

void GameManager::Render()
{
    window->clear();

	sf::CircleShape shape(100.f);
	auto greyValue = rand() % 100;
    shape.setFillColor(sf::Color(greyValue, greyValue, greyValue));
	shape.setPosition(100,100);
	
	window->draw(shape);
    window->display();
}

void GameManager::quit()
{
    EconomicEngine::getInstance()->stop();
	economicEngineThread->join();

	if(debugGUI)
	{
	    (*debugGUI)->quit();
	    debugGUIThread->join();
	}
    window->close();
}

void GameManager::notify(Observable* sender)
{
	//TODO
}
