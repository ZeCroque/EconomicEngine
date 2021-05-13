#include "Grid.h"

#include <MovableTrader.h>


bool Node::isOccupied() const
{
    return actor.lock().get();
}

void Node::resetNode()
{
    localGoal = INFINITY;
    globalGoal = INFINITY;
    visited = false;
    parent = nullptr;
}

Grid::Grid() : minCoordinate(0, 0), maxCoordinate(0, 0)
{
}

void Grid::setActorAt(const std::shared_ptr<Workshop> inWorkshop, const int x, const int y)  // NOLINT(performance-unnecessary-value-param)
{
    inWorkshop->x = x;
    inWorkshop->y = y;
    auto* trader = inWorkshop->getTrader();
    if(trader)
    {
        trader->x = x;
        trader->y = y;
    }
    getNodeAt(x, y).actor = std::weak_ptr(inWorkshop);
}

Workshop *Grid::getActorAt(const int x, const int y)
{
    return getNodeAt(x, y).actor.lock().get();
}

bool Grid::isOccupied(const int x, const int y)
{
    if (world.contains(std::pair(static_cast<int>(x & REGION_MAJOR), static_cast<int>(y & REGION_MAJOR))))
    {
        return getNodeAt(x, y).isOccupied();
    }
    return false;
}


Node &Grid::getNodeAt(const int x, const int y)
{
    //Note: operator[] auto instantiate if nothing is found
    auto &chunk = world[std::pair<int, int>(static_cast<int>(x & REGION_MAJOR), static_cast<int>(y & REGION_MAJOR))];
    auto &node = chunk[x & REGION_MINOR][y & REGION_MINOR];
    node.x = x;
    node.y = y;
    return node;
}

void Grid::updateBounds(const int x, const int y)
{
    if (x < minCoordinate.first) minCoordinate.first = x;
    if (y < minCoordinate.second) minCoordinate.second = y;
    if (x > maxCoordinate.first) maxCoordinate.first = x;
    if (y > maxCoordinate.second) maxCoordinate.second = y;
}

const std::pair<int, int> &Grid::getMinCoordinate() const
{
    return minCoordinate;
}

const std::pair<int, int> &Grid::getMaxCoordinate() const
{
    return maxCoordinate;
}
