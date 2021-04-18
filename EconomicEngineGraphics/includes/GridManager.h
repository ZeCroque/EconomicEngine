//
// Created by relin on 12/04/2021.
//

#ifndef ECONOMICENGINE_GRIDMANAGER_H
#define ECONOMICENGINE_GRIDMANAGER_H


#include <vector>
#include "Grid.h"


class GridManager {
    friend class GameManager;	// Debug
    Grid grid;
    int minRange;
    int parcourStep;

public:
    explicit GridManager();

    void init();
	
    bool canPlaceWorkshop(int x, int y);

    void placeWorkshop(int x, int y, std::vector<std::shared_ptr<StaticActor>> &newWorkshops);

    void makeDebugFile();
};


#endif //ECONOMICENGINE_GRIDMANAGER_H
