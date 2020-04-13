#include "JobManager.h"

#include <utility>

std::string JobManager::getJobName() const
{
	return this->jobName;
}

void JobManager::setJobName(const std::string jobName)
{
	this->jobName = jobName;
}

size_t JobManager::getJobId() const
{
	return this->jobId;
}

void JobManager::setJobId(const size_t jobId)
{
	this->jobId = jobId;
}
