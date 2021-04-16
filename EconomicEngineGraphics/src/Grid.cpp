#include "Grid.h"

void Grid::setActorAt(const std::shared_ptr<Workshop>& staticActor, int x, int y)
{
	getWeakPtrAt(x, y) = std::weak_ptr<Workshop>(staticActor);
}

Workshop* Grid::getActorAt(int x, int y)
{
	return getWeakPtrAt(x, y).lock().get();
}

bool Grid::isOccupied(int x, int y)
{
	return getActorAt(x,y);
}

std::weak_ptr<Workshop>& Grid::getWeakPtrAt(int x, int y)
{
	//Note: operator[] auto instantiate if nothing is found
    Chunk& chunk = world[std::pair<int,int>(x & REGION_MAJOR, y & REGION_MAJOR)];
    return chunk[x & REGION_MINOR][y & REGION_MINOR];
}
