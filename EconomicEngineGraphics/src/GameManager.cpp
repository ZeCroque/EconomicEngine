#include <memory>
#include <filesystem>
#include <fstream>

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

void GameManager::start(const int inTradersAmount)
{
	volatile bool bKeepWaiting = !bIsInitialized;
    while (bKeepWaiting)
    {
	    bKeepWaiting = !bIsInitialized;
    }

    bIsRunning = true;

    EconomicEngine::getInstance()->start(inTradersAmount);

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

void GameManager::reset(const int inTradersAmount)
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
    EconomicEngine::getInstance()->reset(inTradersAmount);

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

GameManager::GameManager() : maxFps(60), window(std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "g_windowTitle")),
                             bIsInitialized(false), bIsRunning(false),
                             bIsPaused(false), bIsGuiOpened(false), bWantsToOpenGui(false), speedFactor(1.f), bMouseDragging(false), bDrawPopup(false),
                             selectedActor(nullptr), bBackgroundNeedsUpdate(true), grassId(0), popupId(0), cellSize(62.f)
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
                    bMouseDragging = true;
                    oldMousePosition = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    mouseClickPosition = oldMousePosition;
                }
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == 0)
                {
                    bMouseDragging = false;
                }
                if (oldMousePosition == mouseClickPosition)
                {
                    getSelectedActor();
                }
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (!bMouseDragging)
                    break;

                const sf::Vector2f newPos = window->mapPixelToCoords(
                        sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                const sf::Vector2f deltaPos = oldMousePosition - newPos;

                const auto& grid = gridManager.getGrid();
                const auto viewOrigin = (view.getCenter() - view.getSize() / 2.f);
                const auto margin = cellSize * 10.f;

                if (viewOrigin.x + deltaPos.x + margin >=
                    static_cast<float>(grid.getMinCoordinate().first) * cellSize &&
                    viewOrigin.x + view.getSize().x + deltaPos.x - margin <=
                    static_cast<float>(grid.getMaxCoordinate().first) * cellSize)
                {
                    view.setCenter(view.getCenter().x + deltaPos.x, view.getCenter().y);
                    window->setView(view);
                }

                if (viewOrigin.y + deltaPos.y + margin >=
                    static_cast<float>(grid.getMinCoordinate().second) * cellSize &&
                    viewOrigin.y + view.getSize().y + deltaPos.y - margin <=
                    static_cast<float>(grid.getMaxCoordinate().second) * cellSize)
                {
                    view.setCenter(view.getCenter().x, view.getCenter().y + deltaPos.y);
                    window->setView(view);
                }

                oldMousePosition = window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                bBackgroundNeedsUpdate = true;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                const auto& grid = gridManager.getGrid();
                const auto newOrigin = (view.getCenter() - (view.getSize() * 1.10f) / 2.f);
                const auto margin = cellSize * 5.f;

                // ReSharper disable once CppTooWideScope
                const bool canUnZoom =
                        newOrigin.x + margin >= static_cast<float>(grid.getMinCoordinate().first) * cellSize &&
                        newOrigin.y + margin >= static_cast<float>(grid.getMinCoordinate().second) * cellSize &&
                        newOrigin.x + view.getSize().x - margin <=
                        static_cast<float>(grid.getMaxCoordinate().first) * cellSize &&
                        newOrigin.y + view.getSize().y - margin <=
                        static_cast<float>(grid.getMaxCoordinate().second) * cellSize;

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
	const auto viewXMin = viewOrigin.x - cellSize;
	const auto viewXMax = viewOrigin.x + view.getSize().x;
	const auto viewYMin = viewOrigin.y - cellSize;
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

void GameManager::getSelectedActor()
{
    auto grid = gridManager.getGrid();

    auto x = static_cast<int>(mouseClickPosition.x / cellSize);
    auto y = static_cast<int>(mouseClickPosition.y / cellSize);

    if (mouseClickPosition.x < 0 && mouseClickPosition.y > 0)
    {
        --x;
    }
    else if ((mouseClickPosition.x < 0 && mouseClickPosition.y < 0))
    {
        --x;
        --y;
    }
    else if ((mouseClickPosition.x > 0 && mouseClickPosition.y < 0))
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

const sf::Texture &GameManager::getTexture(const size_t inTextureId) const
{
    return texturesDictionary[inTextureId];
}

void GameManager::renderBackground(const sf::Vector2<float> inViewOrigin, const float inViewXMin, const float inViewXMax, const float inViewYMin,
                                   const float inViewYMax) const
{
    if (bBackgroundNeedsUpdate)
    {
        background.clear();
        background.create(static_cast<unsigned int>(view.getSize().x), static_cast<unsigned int>(view.getSize().y));
        background.setView(view);

        const auto& grid = gridManager.getGrid();

        const auto groundYMax = static_cast<int>(
                std::max(static_cast<float>(grid.getMaxCoordinate().second), inViewYMax / cellSize) + 1.0f);
        const auto groundXMax = static_cast<int>(
                std::max(static_cast<float>(grid.getMaxCoordinate().first), inViewXMax / cellSize) + 1.0f);
        const auto groundYMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().second),
                                                          inViewYMin / cellSize));
        const auto groundXMin = static_cast<int>(std::min(static_cast<float>(grid.getMinCoordinate().first),
                                                          inViewXMin / cellSize));


        sf::Sprite groundSprite;
        groundSprite.setTexture(getTexture(grassId));

        for (int y = groundYMin; y < groundYMax; ++y)
        {
            for (int x = groundXMin; x < groundXMax; ++x)
            {
                groundSprite.setPosition(static_cast<float>(x) * cellSize, static_cast<float>(y) * cellSize);
                background.draw(groundSprite);
            }
        }

        for (auto& ws : workshops)
        {
            if (auto positionX = static_cast<float>(ws->x) * cellSize, positionY = static_cast<float>(ws->y) * cellSize;
                    positionX >= inViewXMin && positionX <= inViewXMax && positionY >= inViewYMin && positionY <= inViewYMax)
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
    backgroundSprite.setPosition(inViewOrigin);
    window->draw(backgroundSprite);
}

void GameManager::renderTrader(const float inViewXMin, const float inViewXMax, const float inViewYMin, const float inViewYMax) const
{
    if (!bIsPaused)
    {
        for (auto& trader : traders)
        {
            auto& traderSprite = trader->getSprite();
            traderSprite.setScale(1.f, 1.f);
            traderSprite.setOrigin(16.f, 16.f);

            if (auto x = static_cast<float>(trader->x) * cellSize, y = static_cast<float>(trader->y) * cellSize;
                    x >= inViewXMin && x <= inViewXMax && y >= inViewYMin && y <= inViewYMax)
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

            if (trader->coordinatesOffset >= cellSize)
            {
                trader->updatePath();
            }

        }
    }

}

void GameManager::renderPopup() const
{
    if (bDrawPopup)
    {
        auto marketPos = selectedActor->getClosestMarketCoordinate();

        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(cellSize, cellSize));
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineThickness(4);
        rectangle.setOutlineColor(sf::Color::Red);
        rectangle.setPosition(static_cast<float>(marketPos.first) * cellSize,
                              static_cast<float>(marketPos.second) * cellSize);

        window->draw(rectangle);

        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setPosition(static_cast<float>(selectedActor->x) * cellSize,
                              static_cast<float>(selectedActor->y) * cellSize);

        window->draw(rectangle);

        auto trader = selectedActor->getTrader();
        auto x = static_cast<float>(trader->x) * cellSize;
        auto y = static_cast<float>(trader->y) * cellSize;
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
        sf::String money = std::to_string(round(trader->getBoundTrader()->getMoneyCount()));
        sf::String foodLevel = std::to_string(round(trader->getBoundTrader()->getFoodLevel()));
        sf::String action;
        if (trader->getBoundTrader()->getCurrentCraft())
        {
            auto craft = trader->getBoundTrader()->getCurrentCraft();
            action = "Crafting:\n" + EconomicEngine::getInstance()->getTradableFactory().getDefaultObject(
                    craft->getResultId())->getName() + " x" + std::to_string(craft->getCount());
        }
        else if (trader->getBoundTrader()->getPosition() == Position::Market &&
                 !trader->getBoundTrader()->getCurrentAsks().empty())
        {
			std::vector<std::pair<std::basic_string<char>, int>> items;
			for (const auto& ask : trader->getBoundTrader()->getCurrentAsks())
			{
			    auto name = EconomicEngine::getInstance()->getTradableFactory().getDefaultObject(
			            ask->getItemId())->getName();

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

std::shared_ptr<Workshop> GameManager::addWorkshop(const size_t inWorkshopId) const
{
    auto workshop = std::shared_ptr<Workshop>(workshopFactory.createObject(inWorkshopId));
    workshops.push_back(workshop);
    return workshop;
}

std::shared_ptr<Workshop> GameManager::addWorkshop(const std::string& inWorkshopName) const
{
    const std::hash<std::string> hash;
    return addWorkshop(hash(inWorkshopName));
}

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const size_t inWorkshopId) const
{
    auto movableTrader = std::shared_ptr<MovableTrader>(movableTraderFactory.createObject(inWorkshopId));
    traders.push_back(movableTrader);
    return movableTrader;
}

std::shared_ptr<MovableTrader> GameManager::addMovableTrader(const std::string& inWorkshopName) const
{
    const std::hash<std::string> hash;
    return addMovableTrader(hash(inWorkshopName));
}

Workshop* GameManager::findAvailableWorkshop(const size_t inJobId) const
{
    const auto result = std::ranges::find_if(workshops.begin(), workshops.end(),[inJobId](const std::shared_ptr<Workshop> &ws)
	{
		return ws->getJobId() == inJobId && ws->isAvailable();
	});

    return result == workshops.end() ? nullptr : result->get();
}

const GridManager &GameManager::getGridManager() const
{
    return gridManager;
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
            trader->coordinatesOffset += cellSize * inDeltaTime;
        }
    }
}