#ifndef MOVABLE_ACTOR
#define MOVABLE_ACTOR

#include <Signal.h>

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
        reversePath = false;
        pathIterator = path.begin();
        direction = Direction::None;
        coordinatesOffset = 0.f;
    }

    Actor *clone() override;

    const Signal<bool>& getPathfindEndedSignal() const;

	Direction getDirection() const;
	
    const std::pair<int, int>& getNextCoordinate();

    void updatePath();

    void startPathfind(bool inReversePath);

    std::list<std::pair<int, int>> path; //Todo: Make private

    Direction direction;

    float coordinatesOffset;


protected:
    Signal<bool> pathfindEndedSignal;

    std::list<std::pair<int, int>>::iterator pathIterator;

    bool reversePath;

private:
    void updateDirection();
};


#endif //STATIC_ACTOR