#include "JobFactory.h"

std::list<Job*> JobFactory::getJobs() const
{
	std::list<Job*> result;
	for(auto key : getKeys())
	{
		result.emplace_back(getDefaultObject(key));
	}	
	return result;
}
