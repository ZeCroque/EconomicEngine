#include "WorkshopFactory.h"

#include "Workshop.h"

size_t WorkshopFactory::getIdByJobId(size_t inJobId) const
{
	const auto& keys = getKeys();
	for(const auto& key : keys)
	{
		
		const auto& foundId = getDefaultObject(key)->getJobId();
		if(foundId == inJobId)
		{
			return key;
		}
	}
	return 0;
}
