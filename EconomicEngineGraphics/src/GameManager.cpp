#include <memory>
#include <filesystem>
#include <fstream>
#include <QApplication>

#include "GameManager.h"
#include "EconomicEngineDebugGUI.h"
#include "MovableTrader.h"
#include "NavigationSystem.h"
#include "Workshop.h"

void GameManager::init(const char* inContentPath)
{
    assert(std::filesystem::exists(inContentPath) && std::filesystem::is_directory(inContentPath));

    std::vector<nlohmann::json> parsedMovableTraders;
    std::vector<nlohmann::json> parsedWorkshops;

    for (std::ifstream fileStream; const auto& entry : std::filesystem::recursive_directory_iterator(inContentPath))
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
            else if (entry.path().extension() == ".ttf")
            {
                font.loadFromFile(entry.path().string());
            }
        }
    }

    const std::hash<std::string> hash;
    grassId = hash("TX_Grass.png");
    popupId = hash("TX_Popup.png");

    initMovableTraders(parsedMovableTraders);
    initWorkshops(parsedWorkshops);
    initEconomicEngine(inContentPath);
    initGui();
    gridManager.init();
}

void GameManager::exec()
{
	volatile bool bKeepWaiting = !bIsInitialized;
    while (bKeepWaiting)
    {
	    bKeepWaiting = !bIsInitialized;
    }

    bIsRunning = true;

    EconomicEngine::getInstance()->start(50);

    const sf::Clock clock;
    auto previousTimestamp = clock.getElapsedTime().asMicroseconds();
    sf::Int64 lag = 0;

    const auto deltaTime = sf::seconds(1.f / static_cast<float>(maxFps));
    const auto deltaTimeUs = deltaTime.asMicroseconds();
    const auto deltaTimeS = deltaTime.asSeconds();

    while (window->isOpen())
    {
    	processInput();
    	
	    if(const auto currentTimestamp = clock.getElapsedTime().asMicroseconds(); !bIsPaused)
		{
        	const auto timeSinceLastFrame = currentTimestamp - previousTimestamp;
	        previousTimestamp = currentTimestamp;
	        lag += timeSinceLastFrame;

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
	bIsPaused = false;
}

void GameManager::reset(const int inTradersCount)
{	
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

	bDrawPopup = false;
	const auto width = static_cast<float>(window->getSize().x);
	const auto height = static_cast<float>(window->getSize().y);
	view = sf::View(sf::FloatRect(-width / 2.f, -height / 2.f, width, height));
	bBackgroundNeedsUpdate = true;
}

void GameManager::pause()
{
	bIsPaused = true;
}

bool GameManager::isRunning() const
{
    return bIsRunning;
}

bool GameManager::isInitialized() const
{
    return bIsInitialized;
}

void GameManager::setBackgroundNeedsUpdate(const bool inValue) const
{
    bBackgroundNeedsUpdate = inValue;
}

void GameManager::setSpeedFactor(const float inSpeedFactor)
{
    speedFactor = inSpeedFactor;
}

void GameManager::setMaxFps(const int inMaxFps)
{
	maxFps = inMaxFps;
}

float GameManager::getSpeedFactor() const
{
    return speedFactor;
}

int GameManager::getMaxFps() const
{
	return maxFps;
}

const sf::Texture &GameManager::getTexture(const size_t inTextureId) const
{
    return texturesDictionary[inTextureId];
}

GameManager::GameManager() : maxFps(60), window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "g_windowTitle")),
                             bMoving(false), bDrawPopup(false),
                             bIsInitialized(false), bIsRunning(false), bIsPaused(false), bIsGuiOpened(false), bWantsToOpenGui(false), speedFactor(1.f),
                             bBackgroundNeedsUpdate(true), selectedActor(nullptr), grassId(0), popupId(0), caseSize(62.f)
{
    window->setFramerateLimit(maxFps);
    view.setCenter(0, 0);
}


void GameManager::initEconomicEngine(const char* inPrefabsPath)
{
    auto* economicEngine = EconomicEngine::getInstance();
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
				bIsInitialized = true;
			});
			debugGui.show();
			bIsGuiOpened = true;
			bWantsToOpenGui = false;
			result = QApplication::exec();

			bIsGuiOpened = false;
			volatile bool bKeepWaiting = !bWantsToOpenGui && bIsRunning;
			while (bKeepWaiting)
			{
				bKeepWaiting = !bWantsToOpenGui && bIsRunning;
			}
		} while (bIsRunning);

		return result;
	});
}

void GameManager::initMovableTraders(std::vector<nlohmann::json>& inParsedMovableTraders)
{
    for (const std::hash<std::string> hasher; const auto& parsedMovableTrader : inParsedMovableTraders)
    {
        auto* movableTrader = new MovableTrader(parsedMovableTrader["job"], parsedMovableTrader["texture"]);
        movableTraderFactory.registerObject(hasher(parsedMovableTrader["job"]), movableTrader);
    }
}

void GameManager::initWorkshops(std::vector<nlohmann::json>& inParsedWorkshops)
{ 
    for (const std::hash<std::string> hash; const auto& parsedWorkshop : inParsedWorkshops)
    {
        auto* workshop = new Workshop(parsedWorkshop["name"], parsedWorkshop["job"], parsedWorkshop["texture"]);
        workshopFactory.registerObject(hash(parsedWorkshop["name"]), workshop);
    }
}

void GameManager::initTexture(const std::filesystem::path& inPath) const
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
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == 0)
                {
                    bMoving = true;
                    oldPos = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    clickPos = oldPos;
                }
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == 0)
                {
                    bMoving = false;
                }
                if (oldPos == clickPos)
                {
                    getSelectedActor();
                }
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (!bMoving)
                    break;

                const sf::Vector2f newPos = window->mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                const sf::Vector2f deltaPos = oldPos - newPos;

                const auto& grid = gridManager.getGrid();
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
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                const auto& grid = gridManager.getGrid();
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
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    view.setCenter(0, 0);
                    bBackgroundNeedsUpdate = true;
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    if (!bIsGuiOpened)
                    {
                        bWantsToOpenGui = true;
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

	updateWorkshops(inDeltaTime);

	updateTraders(inDeltaTime);
}

void GameManager::render() const
{
	const auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
	const auto viewXMin = viewOrigin.x - caseSize;
	const auto viewXMax = viewOrigin.x + view.getSize().x;
	const auto viewYMin = viewOrigin.y - caseSize;
	const auto viewYMax = viewOrigin.y + view.getSize().y;

    window->clear();

    renderBackground(viewOrigin, viewXMin, viewXMax, viewYMin, viewYMax);
    renderTrader(viewXMin, viewXMax, viewYMin, viewYMax);
    renderPopup();

    window->display();
}

void GameManager::quit()
{
    bIsRunning = false;

    EconomicEngine::getInstance()->getStockExchange().getAskResolvedSignal().disconnectAll();

    if (debugGuiThread)
    {
        if (bIsGuiOpened)
        {
            QApplication::quit();
        }
        debugGuiThread->join();
    }

    gridManager.getGenerationThread().join();
    window->close();
}

void GameManager::traderAddedCallback(Trader *inTrader)
{
    auto* movableTrader = movableTraderFactory.createObject(inTrader->getCurrentJob()->getId());
    movableTrader->setBoundTrader(inTrader);
    pendingTraders.push(movableTrader);
    inTrader->getMoveToRequestSignal().connect([inTrader, movableTrader](const Position inPosition)
	{
	   movableTrader->moveToRequestCallback(inTrader, inPosition);
	});

    inTrader->getDeathSignal().connect([movableTrader, this]()
	{
		movableTrader->getPathfindEndedSignal().disconnectAll();
		traders.remove_if([movableTrader](const std::shared_ptr<MovableTrader> &inMovableTrader)
		{
			return inMovableTrader.get() == movableTrader;
		});
	});
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const size_t inKey) const
{
    auto workshop = std::shared_ptr<Workshop>(workshopFactory.createObject(inKey));
    workshops.push_back(workshop);
    return workshop;
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const std::string& inName) const
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

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const std::string& inName) const
{
    const std::hash<std::string> hash;
    return addMovableTrader(hash(inName));
}

Workshop* GameManager::findAvailableWorkshop(const size_t inJobId) const
{
    const auto result = std::ranges::find_if(workshops.begin(), workshops.end(),[inJobId](const std::shared_ptr<Workshop> &ws)
	{
		return ws->getJobId() == inJobId && ws->isAvailable();
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
        bDrawPopup = true;
    }
    else
    {
        bDrawPopup = false;
    }

}

const GridManager &GameManager::getGridManager() const
{
    return gridManager;
}

void GameManager::updateWorkshops(const float inDeltaTime)
{
    while (!pendingTraders.empty())
    {
        auto trader = std::shared_ptr<MovableTrader>(pendingTraders.front());
        pendingTraders.pop();
        traders.push_back(trader);

        if (auto* availableWorkshop = findAvailableWorkshop(trader->getJobId()); availableWorkshop)
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

void GameManager::updateTraders(const float inDeltaTime) const
{
    for (auto &trader : traders)
    {
        if (trader->direction != Direction::None)
        {
            trader->coordinatesOffset += caseSize * inDeltaTime;
        }
    }
}

void GameManager::renderPopup() const
{
    if (bDrawPopup)
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
        rectangle.setPosition(x + 16, y + 32);

        window->draw(rectangle);


        sf::Sprite popupSprite;
        popupSprite.setTexture(getTexture(popupId));
        popupSprite.setPosition(10, 0);
        window->setView(sf::View(sf::FloatRect(0, 0,
                                               static_cast<float>(window->getSize().x),
                                               static_cast<float>(window->getSize().y))));
        window->draw(popupSprite);


        sf::Text text;
        text.setFont(font);
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(12);

        text.setPosition(40, 32);
        text.setString("Job:");
        window->draw(text);

        text.setPosition(40, 64);
        text.setString("Money:");
        window->draw(text);

        text.setPosition(40, 96);
        text.setString("Food:");
        window->draw(text);

        text.setPosition(40, 128);
        text.setString("Action:");
        window->draw(text);


        sf::String jobName = trader->getBoundTrader()->getCurrentJob()->getName();
        sf::String money = std::to_string(round(trader->getBoundTrader()->getMoney()));
        sf::String foodLevel = std::to_string(round(trader->getBoundTrader()->getFoodLevel()));
        sf::String action;
        if (trader->getBoundTrader()->getCurrentCraft())
        {
            auto craft = trader->getBoundTrader()->getCurrentCraft();
            action = "Crafting:\n" + EconomicEngine::getInstance()->getTradableFactory().getDefaultObject(
                    craft->getResult())->getName() + " x" + std::to_string(craft->getCount());
        }
        else if (trader->getBoundTrader()->getPosition() == Position::Market &&
                 !trader->getBoundTrader()->getCurrentAsks().empty())
        {
			std::vector<std::pair<std::basic_string<char>, int>> items;
			for (const auto& ask : trader->getBoundTrader()->getCurrentAsks())
			{
			    auto name = EconomicEngine::getInstance()->getTradableFactory().getDefaultObject(
			            ask->getId())->getName();

			    auto findAny = false;
			    for (auto& item : items)
			    {
			        if (name == item.first)
			        {
			            ++item.second;
			            findAny = true;
			            break;
			        }
			    }
			    if (!findAny)
			    {
			        items.emplace_back(std::pair<std::basic_string<char>, int>(name, 1));
			    }
			}

            action = "Buying:\n";
            for (auto const& item : items)
            {
                action += item.first + " x" + std::to_string(item.second) + "\n";
            }
        }
        else if (trader->getBoundTrader()->getPosition() == Position::Street)
        {
            if (trader->getBoundTrader()->getCurrentAction() == Action::Trading)
            {
                action = "Going to:\n market";
            }
            else if (trader->getBoundTrader()->getCurrentAction() == Action::Crafting)
            {
                action = "Going to\n workshop";
            }
        }
        else
        {
            action = "Waiting";
        }

        text.setStyle(sf::Text::Regular);

        text.setPosition(75, 32);
        text.setString(jobName);
        window->draw(text);

        text.setPosition(90, 64);
        text.setString(money);
        window->draw(text);

        text.setPosition(85, 96);
        text.setString(foodLevel);
        window->draw(text);

        text.setPosition(100, 128);
        text.setString(action);
        window->draw(text);


        window->setView(view);
    }
}

void GameManager::renderTrader(const float viewXMin, const float viewXMax, const float viewYMin, const float viewYMax) const
{
    if (!bIsPaused)
    {
        for (auto& trader : traders)
        {
            auto& traderSprite = trader->getSprite();
            traderSprite.setScale(1.f, 1.f);
            traderSprite.setOrigin(16.f, 16.f);

            if (auto x = static_cast<float>(trader->x) * caseSize, y = static_cast<float>(trader->y) * caseSize;
                    x >= viewXMin && x <= viewXMax && y >= viewYMin && y <= viewYMax)
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

}

void GameManager::renderBackground(const sf::Vector2<float> viewOrigin, const float viewXMin, const float viewXMax, const float viewYMin,
                                   const float viewYMax) const
{
    if (bBackgroundNeedsUpdate)
    {
        background.clear();
        background.create(static_cast<unsigned int>(view.getSize().x), static_cast<unsigned int>(view.getSize().y));
        background.setView(view);

        const auto& grid = gridManager.getGrid();

        const auto groundYMax = static_cast<int>(
                std::max(static_cast<float>(grid.getMaxCoordinate().second), viewYMax / caseSize) + 1.0f);
        const auto groundXMax = static_cast<int>(
                std::max(static_cast<float>(grid.getMaxCoordinate().first), viewXMax / caseSize) + 1.0f);
        const auto groundYMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().second),
                                                          viewYMin / caseSize));
        const auto groundXMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().first),
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

        for (auto& ws : workshops)
        {
            if (auto positionX = static_cast<float>(ws->x) * caseSize, positionY = static_cast<float>(ws->y) * caseSize;
                    positionX >= viewXMin && positionX <= viewXMax && positionY >= viewYMin && positionY <= viewYMax)
            {
                auto& workshopSprite = ws->getSprite();
                workshopSprite.setPosition(positionX, positionY);
                background.draw(workshopSprite);
            }
        }

        background.display();
        bBackgroundNeedsUpdate = false;
    }


    const sf::Texture& texture = background.getTexture();
    sf::Sprite backgroundSprite(texture);
    backgroundSprite.setPosition(viewOrigin);
    window->draw(backgroundSprite);
}


