#include "Grid.h"
#include "MovableTrader.h"

bool Node::isOccupied() const
{
    return actor.lock().get();
}

void Node::resetNode()
{
    localGoal = INFINITY;
    globalGoal = INFINITY;
    bVisited = false;
    parent = nullptr;
}

Grid::Grid() : minCoordinate(0, 0), maxCoordinate(0, 0)
{
}

void Grid::reset()
{
	minCoordinate = std::pair(0,0);
	maxCoordinate = std::pair(0,0);
}

void Grid::updateBounds(const int inX, const int inY)
{
    if (inX < minCoordinate.first)
    {
    	minCoordinate.first = inX;
    }
    if (inY < minCoordinate.second)
    {
    	minCoordinate.second = inY;
    }
    if (inX > maxCoordinate.first)
    {
    	maxCoordinate.first = inX;
    }
    if (inY > maxCoordinate.second)
    {
    	maxCoordinate.second = inY;
    }
}

bool Grid::isOccupied(const int inX, const int inY)
{
    if (world.contains(std::pair(static_cast<int>(inX & REGION_MAJOR), static_cast<int>(inY & REGION_MAJOR))))
    {
        return getNodeAt(inX, inY).isOccupied();
    }
    return false;
}

Workshop* Grid::getActorAt(const int inX, const int inY)
{
    return getNodeAt(inX, inY).actor.lock().get();
}

Node& Grid::getNodeAt(const int inX, const int inY)
{
    //Note: operator[] auto instantiate if nothing is found
    auto &chunk = world[std::pair<int, int>(static_cast<int>(inX & REGION_MAJOR), static_cast<int>(inY & REGION_MAJOR))];
    auto &node = chunk[inX & REGION_MINOR][inY & REGION_MINOR];
    node.x = inX;
    node.y = inY;
    return node;
}

const std::pair<int, int>& Grid::getMinCoordinate() const
{
    return minCoordinate;
}

const std::pair<int, int>& Grid::getMaxCoordinate() const
{
    return maxCoordinate;
}

void Grid::setActorAt(const std::shared_ptr<Workshop> inWorkshop, const int inX,  const int inY)  // NOLINT(performance-unnecessary-value-param)
{
    inWorkshop->x = inX;
    inWorkshop->y = inY;
    if(auto* trader = inWorkshop->getTrader(); trader)
    {
        trader->x = inX;
        trader->y = inY;
    }
    getNodeAt(inX, inY).actor = std::weak_ptr(inWorkshop);
}
