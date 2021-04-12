#ifndef GRID_H
#define GRID_H

#include <map>
#include <array>
#include <memory>

class StaticActor;

constexpr size_t REGION_SIZE = 16;
constexpr size_t REGION_MINOR = REGION_SIZE - 1;
constexpr size_t REGION_MAJOR = ~REGION_MINOR;

typedef std::array<std::array<std::weak_ptr<StaticActor>,  REGION_SIZE>,  REGION_SIZE> Chunk;

class Grid
{
public:
	void setActorAt(const std::shared_ptr<StaticActor>&& staticActor, int x, int y);
	StaticActor* getActorAt(int x, int y); 
	bool isOccupied(int x, int y);

private:
	std::weak_ptr<StaticActor> getWeakPtrAt(int x, int y);
	
	std::map<std::pair<int, int>, Chunk> world;
};

#endif //GRID_H
