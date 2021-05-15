#include "Traders/JobFactory.h"

std::list<Job*> JobFactory::getJobsDefaultObjects() const
{
	std::list<Job*> jobs;
	for(const auto& jobId : getKeys())
	{
		jobs.emplace_back(getDefaultObject(jobId));
	}	
	return jobs;
}
