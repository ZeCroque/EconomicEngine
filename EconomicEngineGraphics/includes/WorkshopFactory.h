#ifndef WORKSHOP_FACTORY
#define WORKSHOP_FACTORY
#include "AbstractFactory.h"

class Workshop;

class WorkshopFactory : public AbstractFactory<size_t, Workshop>
{
    friend class AbstractFactory<size_t, Workshop>;

public:
    size_t getIdByJobId(size_t jobId);
};

#endif //WORKSHOP_FACTORY