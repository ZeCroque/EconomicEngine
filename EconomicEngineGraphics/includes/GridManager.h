//
// Created by relin on 12/04/2021.
//

#ifndef ECONOMICENGINE_GRIDMANAGER_H
#define ECONOMICENGINE_GRIDMANAGER_H


#include <vector>
#include "Grid.h"
#include <queue>


class GridManager {
    Grid grid;
    int minRange;
    int parcourStep;

    std::queue<std::shared_ptr<Workshop>> workshopQueue;

    std::pair<int, int> minCoordinate;
    std::pair<int, int> maxCoordinate;
    std::thread generationThread;

    void updateBound(int x, int y);

    void placeWorkshop();

public:

    explicit GridManager();

    void init();

    bool canPlaceWorkshop(int x, int y);

    void queueWorkshop(std::shared_ptr<Workshop> workshop);

    [[nodiscard]] std::thread &getGenerationThread();

    void makeDebugFile();


};


#endif //ECONOMICENGINE_GRIDMANAGER_H
