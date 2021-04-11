#include "GameManager.h"

const sf::Int32 GameManager::maxFPS = 60;

GameManager::~GameManager()
{
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

GameManager::GameManager() : window(std::make_unique<sf::RenderWindow>(sf::VideoMode::getFullscreenModes()[0], "g_windowTitle", sf::Style::Fullscreen))
{
	window->setFramerateLimit(maxFPS);
}

void GameManager::ProcessInput()
{
	sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
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