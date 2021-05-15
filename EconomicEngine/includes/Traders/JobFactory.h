#ifndef JOB_FACTORY_H
#define JOB_FACTORY_H

#include "AbstractFactory.h"
#include "Job.h"

class JobFactory : public AbstractFactory<size_t, Job>
{
public:
	[[nodiscard]] std::list<Job*> getJobsDefaultObjects() const;
};

#endif //JOB_FACTORY_H

