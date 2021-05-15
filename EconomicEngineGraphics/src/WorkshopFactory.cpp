#include "WorkshopFactory.h"
#include "Workshop.h"

size_t WorkshopFactory::getIdByJobId(const size_t inJobId) const
{
	for(const auto & keys = getKeys(); const auto& key : keys)
	{
		if(const auto & foundId = getDefaultObject(key)->getJobId(); foundId == inJobId)
		{
			return key;
		}
	}
	return 0;
}
