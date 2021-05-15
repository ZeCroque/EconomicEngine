#include <cassert>

#include "MovableActor.h"

Actor* MovableActor::clone()
{
    return new MovableActor(*this);
}

const Signal<bool>& MovableActor::getPathfindEndedSignal() const
{
    return pathfindEndedSignal;
}

Direction MovableActor::getDirection() const
{
	return direction;
}

const std::pair<int, int>& MovableActor::getNextCoordinate() const
{
    assert(pathIterator != path.end());
    return *pathIterator;
}

void MovableActor::updatePath()
{
    switch (direction)  // NOLINT(clang-diagnostic-switch-enum)
    {
        case Direction::Top:
            --y;
            break;
        case Direction::Bottom:
            ++y;
            break;
        case Direction::Left:
            --x;
            break;
        case Direction::Right:
            ++x;
            break;
        default:;
    }

    coordinatesOffset = 0.f;
	if (bReversePath && pathIterator != path.begin())
    {
        --pathIterator;
    }
	else if(!bReversePath)
    {
        ++pathIterator;
    }
	updateDirection();
}

void MovableActor::startPathfind(const bool inReversePath)
{
    bReversePath = inReversePath;
    pathIterator = !bReversePath ? ++path.begin() : ----path.end();
    coordinatesOffset = 0.f;
    updateDirection();
}

void MovableActor::updateDirection()
{
	if(pathIterator == path.end() || (pathIterator == path.begin() && pathIterator->first == x && pathIterator->second == y))
	{   	
		direction = Direction::None;
		pathfindEndedSignal(true);
	}
	else if (pathIterator->first < x)
	{
		direction = Direction::Left;
	}
	else if (pathIterator->first > x)
	{
		direction = Direction::Right;
	}
	else if (pathIterator->second < y)
	{
		direction = Direction::Top;
	}
	else if (pathIterator->second > y)
	{
		direction = Direction::Bottom;
	}
}
