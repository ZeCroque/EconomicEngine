#ifndef MOVABLE_ACTOR_H
#define MOVABLE_ACTOR_H

#include "Signal.h"
#include "Actor.h"

enum class Direction
{
    None,
    Top,
    Bottom,
    Left,
    Right
};

class MovableActor : public Actor
{

public:
    explicit MovableActor(const std::string &inTextureName) : Actor(inTextureName)
    {
        bReversePath = false;
        pathIterator = path.begin();
        direction = Direction::None;
        coordinatesOffset = 0.f;
    }

    Actor* clone() override;

    const Signal<bool>& getPathfindEndedSignal() const;

	Direction getDirection() const;
	
    const std::pair<int, int>& getNextCoordinate() const;

    void updatePath();

    void startPathfind(bool inReversePath);

    Direction direction;

    float coordinatesOffset;

protected:
    Signal<bool> pathfindEndedSignal;

    std::list<std::pair<int, int>>::iterator pathIterator;

    std::list<std::pair<int, int>> path;
	
    bool bReversePath;

private:
    void updateDirection();
};


#endif //MOVABLE_ACTOR_H