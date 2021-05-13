#ifndef GRID_H
#define GRID_H

#include <map>
#include <array>
#include <memory>
#include <Workshop.h>
#include <vector>


constexpr size_t REGION_SIZE = 16;
constexpr size_t REGION_MINOR = REGION_SIZE - 1;
constexpr size_t REGION_MAJOR = ~REGION_MINOR;

struct Node {
    int x = 0;
    int y = 0;
    bool visited = false;
    float globalGoal = INFINITY;
    float localGoal = INFINITY;
    Node *parent = nullptr;
    std::weak_ptr<Workshop> actor;

    [[nodiscard]] bool isOccupied() const;

    void resetNode();
};

typedef std::array<std::array<Node, REGION_SIZE>, REGION_SIZE> Chunk;


class Grid {
public:
    Grid();

    void setActorAt(std::shared_ptr<Workshop> inWorkshop, int x, int y);

    Workshop *getActorAt(int x, int y);

    bool isOccupied(int x, int y);

    Node &getNodeAt(int x, int y);

    void updateBounds(int x, int y);

    [[nodiscard]] const std::pair<int, int> &getMinCoordinate() const;

    [[nodiscard]] const std::pair<int, int> &getMaxCoordinate() const;

private:
    std::map<std::pair<int, int>, Chunk> world;

    std::pair<int, int> minCoordinate;
    std::pair<int, int> maxCoordinate;
};

#endif //GRID_H
