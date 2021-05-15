#ifndef GRID_H
#define GRID_H

#include <map>
#include <array>
#include <memory>

#include "Workshop.h"

constexpr size_t REGION_SIZE = 16;
constexpr size_t REGION_MINOR = REGION_SIZE - 1;
constexpr size_t REGION_MAJOR = ~REGION_MINOR;

struct Node
{
    [[nodiscard]] bool isOccupied() const;

    void resetNode();

	int x = 0;
    int y = 0;
    bool bVisited = false;
    float globalGoal = INFINITY;
    float localGoal = INFINITY;
    Node* parent = nullptr;
    std::weak_ptr<Workshop> actor;
};

typedef std::array<std::array<Node, REGION_SIZE>, REGION_SIZE> Chunk;

class Grid
{
public:
    Grid();

    void reset();
  
    void updateBounds(int inX, int inY);
	
    bool isOccupied(int inX, int inY);

    Workshop* getActorAt(int inX, int inY);

    Node& getNodeAt(int inX, int inY);

    [[nodiscard]] const std::pair<int, int>& getMinCoordinate() const;

    [[nodiscard]] const std::pair<int, int>& getMaxCoordinate() const;

	void setActorAt(std::shared_ptr<Workshop> inWorkshop, int inX, int inY);

private:
    std::map<std::pair<int, int>, Chunk> world;

    std::pair<int, int> minCoordinate;
    std::pair<int, int> maxCoordinate;
};

#endif //GRID_H
