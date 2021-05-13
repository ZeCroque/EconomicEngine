#ifndef WORKSHOP_FACTORY
#define WORKSHOP_FACTORY
#include "AbstractFactory.h"
#include "Workshop.h"


class WorkshopFactory : public AbstractFactory<size_t, Workshop>
{
    friend class AbstractFactory<size_t, Workshop>;

public:
    [[nodiscard]] size_t getIdByJobId(size_t inJobId) const;
};

#endif //WORKSHOP_FACTORY