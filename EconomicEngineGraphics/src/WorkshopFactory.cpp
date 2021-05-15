#include "WorkshopFactory.h"
#include "Workshop.h"

size_t WorkshopFactory::getIdByJobId(const size_t inJobId) const
{
	for(const auto& workshopId : getKeys())
	{
		if(const auto & foundId = getDefaultObject(workshopId)->getJobId(); foundId == inJobId)
		{
			return workshopId;
		}
	}
	return 0;
}
