//
// Created by relin on 12/04/2021.
//

#ifndef ECONOMICENGINE_GRIDMANAGER_H
#define ECONOMICENGINE_GRIDMANAGER_H


#include <vector>
#include "Grid.h"


class GridManager {

    Grid grid;
    int minRange;
    int parcourStep;

    std::pair<int,int> minCoordinate;
    std::pair<int,int> maxCoordinate;


    void updateBound(int x,int y);

public:
    explicit GridManager();

    void init();
	
    bool canPlaceWorkshop(int x, int y);

    void placeWorkshop(int x, int y, std::vector<std::shared_ptr<StaticActor>> &newWorkshops);

    void makeDebugFile();
};


#endif //ECONOMICENGINE_GRIDMANAGER_H
