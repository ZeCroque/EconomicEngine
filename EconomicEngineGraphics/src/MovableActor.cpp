//
// Created by relin on 14/04/2021.
//

#include <MovableActor.h>
#include <cassert>

Actor *MovableActor::clone()
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

const std::pair<int, int> &MovableActor::getNextCoordinate()
{
    //assert(pathIterator != path.begin() && pathIterator != path.end());
    return *pathIterator;
}

void MovableActor::updatePath()
{
    switch (direction)
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
    if (reversePath && pathIterator != path.begin())
    {
        --pathIterator;
    }
    else if (!reversePath)
    {
        ++pathIterator;
    }

    if (pathIterator != path.end())
    {
        updateDirection();
    }else{
        direction = Direction::None;
        pathfindEndedSignal(true);
    }
}

void MovableActor::startPathfind(bool inReversePath)
{
    reversePath = inReversePath;
    pathIterator = !reversePath ? ++path.begin() : ----path.end();
    coordinatesOffset = 0.f;
    updateDirection();
}

void MovableActor::updateDirection()
{
    if (pathIterator->first < x)
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
    }else{
        direction = Direction::None;
    	pathfindEndedSignal(true);
    }
}
