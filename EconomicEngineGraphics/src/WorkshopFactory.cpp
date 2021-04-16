#include "WorkshopFactory.h"

#include "Workshop.h"

size_t WorkshopFactory::getIdByJobId(size_t jobId) const
{
	const auto& keys = getKeys();
	for(const auto& key : keys)
	{
		
		const auto& foundId = getDefaultObject(key)->getJobId();
		if(foundId == jobId)
		{
			return key;
		}
	}
	return 0;
}
