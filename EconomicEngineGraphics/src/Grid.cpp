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
    neighbors.clear();
    visited = false;
    parent = nullptr;
}

Grid::Grid() : minCoordinate(0, 0), maxCoordinate(0, 0)
{
}

void Grid::setActorAt(const std::shared_ptr<Workshop> workshop, const int x,  const int y)  // NOLINT(performance-unnecessary-value-param)
{
    workshop->x = x;
    workshop->y = y;
    auto trader = workshop->getTrader();
    if(trader)
    {
        trader->x = x;
        trader->y = y + 1;
        trader->path.emplace_back(std::pair<int,int>(trader->x,trader->y));
        trader->path.emplace_back(std::pair<int,int>(trader->x - 1,trader->y));
        trader->path.emplace_back(std::pair<int,int>(trader->x-1,trader->y - 1));
        trader->startPathfind(false);
    }
    getNodeAt(x, y).actor = std::weak_ptr(workshop);
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
