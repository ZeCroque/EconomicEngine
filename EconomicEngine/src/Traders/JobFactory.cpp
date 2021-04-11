#include "Traders/JobFactory.h"

std::list<Job*> JobFactory::getJobs() const
{
	std::list<Job*> result;
	for(const auto& key : getKeys())
	{
		result.emplace_back(getDefaultObject(key));
	}	
	return result;
}
