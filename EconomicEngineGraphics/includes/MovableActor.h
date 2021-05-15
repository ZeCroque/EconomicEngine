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
    explicit MovableActor(const std::string &inTextureName);

    void startPathfind(bool inReversePath);
	
    void updatePath();
	
    const Signal<bool>& getPathfindEndedSignal() const;

	Direction getDirection() const;
	
    const std::pair<int, int>& getNextCoordinate() const;

	Actor* clone() override;

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