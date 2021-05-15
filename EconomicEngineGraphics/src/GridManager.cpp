#include <random>

#include "GridManager.h"
#include "GameManager.h"

#ifndef NDEBUG
#include <fstream>
#endif
#include "Workshop.h"

GridManager::GridManager() : pathStep(4), minRangeBetweenWorkshops(6), maxDistanceToMarket(25), bIsGenerationThreadRunning(false)
{	
}

void GridManager::init()
{
    grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), 0, 0);
    marketsCoordinates.emplace_back(std::pair<int, int>(0, 0));

	bIsGenerationThreadRunning = true;
	generationThread = std::thread([this]() { placeWorkshops(); });
}

void GridManager::reset()
{
    bIsGenerationThreadRunning = false;
	generationThread.join();
	
	while(!workshopQueue.empty())
	{
		workshopQueue.pop();
	}

	marketsCoordinates.clear();
	grid.reset();
}

bool GridManager::canPlaceWorkshop(const int inX, const int inY) const
{
	const int range = minRangeBetweenWorkshops;
    for (int i = inX - range / 2; i < inX + range / 2; i++) 
    {
        for (int j = inY - range / 2; j < inY + range / 2; j++) 
        {
            if (grid.isOccupied(i, j)) 
            {
                return false;
            }
        }
    }
    return true;
}

void GridManager::queueWorkshop(std::shared_ptr<Workshop> inWorkshop) // NOLINT(performance-unnecessary-value-param)
{
    workshopQueue.emplace(inWorkshop);
}

bool GridManager::isGenerationThreadRunning() const
{
	return bIsGenerationThreadRunning;
}

std::thread& GridManager::getGenerationThread()
{
    return generationThread;
}

Grid& GridManager::getGrid() const
{
	return grid;
}

void GridManager::placeWorkshops()
{
    int d = getRandomInt(0, 4); // current direction; 0=RIGHT, 1=DOWN, 2=LEFT, 3=UP
    int s = 1; // chain size
    int x = 0;
    int y = 0;

    while (true) 
    {
        for (int j = 0; j < 2; j++) 
        {
            for (int i = 0; i < s; i += pathStep) 
            {
                while (workshopQueue.empty() && GameManager::getInstance()->isRunning() && bIsGenerationThreadRunning ||
                       !GameManager::getInstance()->isInitialized()) 
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(GameManager::getInstance()->getMaxFps()));
                }
                if (!GameManager::getInstance()->isRunning() && GameManager::getInstance()->isInitialized() || !bIsGenerationThreadRunning) 
                {
#ifndef NDEBUG
                    makeDebugFile();
#endif
                    return;
                }

                if (auto noiseX = getRandomInt(x - minRangeBetweenWorkshops / 5, x + minRangeBetweenWorkshops / 5), noiseY = getRandomInt(y - minRangeBetweenWorkshops / 5, y + minRangeBetweenWorkshops / 5); !workshopQueue.empty() && canPlaceWorkshop(noiseX, noiseY)) 
                {
                	grid.updateBounds(noiseX, noiseY);
                    if (isAnyMarketInRange(noiseX, noiseY)) 
                    {
                        auto &ws = workshopQueue.front();
                        workshopQueue.pop();
                    	grid.setActorAt(ws, noiseX, noiseY);
                        ws->setClosestMarketCoordinate(getClosestMarket(noiseX, noiseY));
                    }
                	else 
                    {
                    	
                        grid.setActorAt(GameManager::getInstance()->addWorkshop("Market"), noiseX, noiseY);
                        marketsCoordinates.emplace_back(std::pair<int, int>(noiseX, noiseY));
                        updateClosestMarket(noiseX, noiseX);
                    }
                    GameManager::getInstance()->setBackgroundNeedsUpdate(true);
                }

                switch (d)
            	{
                    case 0:
                        y += pathStep;
                        break;
                    case 1:
                        x += pathStep;
                        break;
                    case 2:
                        y -= pathStep;
                        break;
                    case 3:
                        x -= pathStep;
                        break;
                    default:;
                }
            }
            d = (d + 1) % 4;
        }
        s = s + pathStep;
    }
}

void GridManager::updateClosestMarket(const int inX, const int inY)
{

    auto const yMin = std::max(grid.getMinCoordinate().second, inY - maxDistanceToMarket / 2);
    auto const yMax = std::min(grid.getMaxCoordinate().second, inY + maxDistanceToMarket / 2);
    auto const xMin = std::max(grid.getMinCoordinate().first, inX - maxDistanceToMarket / 2);
    auto const xMax = std::min(grid.getMaxCoordinate().first, inX + maxDistanceToMarket / 2);

    for (int y = yMin; y <= yMax; ++y) 
    {
        for (int x = xMin; x <= xMax; ++x) 
        {
	        if (auto * cell = grid.getActorAt(x, y); cell) 
            {
                cell->setClosestMarketCoordinate(getClosestMarket(inX, inY));
            }
        }
    }
}

#ifndef NDEBUG
void GridManager::makeDebugFile() const
{
    std::ofstream file;
    file.open("../result.txt");

    for (int y = grid.getMinCoordinate().second - 1; y < grid.getMaxCoordinate().second; ++y) 
    {
        for (int x = grid.getMinCoordinate().first - 1; x < grid.getMaxCoordinate().first; ++x) 
        {
            if (grid.isOccupied(x, y)) 
            {
                file << grid.getActorAt(x, y)->getId() << "\t";
            }
        	else 
            {
                file << 0 << "\t";
            }
        }
        file << std::endl;
    }
    file.close();
}
#endif

bool GridManager::isAnyMarketInRange(const int inX, const int inY)
{

    if (std::ranges::any_of(marketsCoordinates.begin(), marketsCoordinates.end(), [inX, inY, this](const std::pair<int, int> coordinates) 
	    {
	        return static_cast<int>(sqrt(pow(coordinates.first - inX, 2) + pow(coordinates.second - inY, 2))) < maxDistanceToMarket;
	    })) 
    {
        return true;
    }

    return false;
}

std::pair<int, int> GridManager::getClosestMarket(const int inX, const int inY)
{
	std::pair<int, int> result;

    for (double bestDistance = INT8_MAX; const auto& market : marketsCoordinates)
    {
	    if (const double distance = sqrt(pow(market.first - inX, 2) + pow(market.second - inY, 2)); distance < bestDistance)
        {
            bestDistance = distance;
            result = market;
        }
    }

    return result;
}

int GridManager::getClosestMarketCoordinate(const int inX, const int inY)
{
    int closest = maxDistanceToMarket + 1;

    for (const auto& marketCoordinates : marketsCoordinates) 
    {
	    if (const auto current = static_cast<int>(sqrt(pow(marketCoordinates.first - inX, 2) + pow(marketCoordinates.second - inY, 2))); current <= closest) 
        {
            closest = current;
        }
    }
    return closest;
}

int GridManager::getRandomInt(const int min, const int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<> distribution(min, max);

    return distribution(gen);
}