#include "Grid.h"



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

Grid::Grid() : minCoordinate(0,0), maxCoordinate(0,0)
{
}

void Grid::setActorAt(const std::shared_ptr<Workshop> workshop, int x, int y)
{
    workshop->x = x;
    workshop->y = y;
	getNodeAt(x, y).actor = std::weak_ptr(workshop);
}

Workshop* Grid::getActorAt(int x, int y)
{
	return getNodeAt(x, y).actor.lock().get();
}

bool Grid::isOccupied(int x, int y)
{
	if(world.contains(std::pair(x & REGION_MAJOR, y & REGION_MAJOR)))
	{
		return getNodeAt(x,y).isOccupied();
	}
	return false;
}


Node& Grid::getNodeAt(int x, int y)
{
	//Note: operator[] auto instantiate if nothing is found
    auto& chunk = world[std::pair<int,int>(x & REGION_MAJOR, y & REGION_MAJOR)];
	auto& node = chunk[x & REGION_MINOR][y & REGION_MINOR];
	node.x = x;
	node.y = y;
	return node;
}

void Grid::updateBounds(int x, int y)
{
	if (x < minCoordinate.first) minCoordinate.first = x;
    if (y < minCoordinate.second) minCoordinate.second = y;
    if (x > maxCoordinate.first) maxCoordinate.first = x;
    if (y > maxCoordinate.second) maxCoordinate.second = y;
}

const std::pair<int, int>& Grid::getMinCoordinate() const
{
	return minCoordinate;
}

const std::pair<int, int>& Grid::getMaxCoordinate() const
{
	return maxCoordinate;
}
