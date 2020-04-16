#include "JobManager.h"

#include <utility>

JobManager::JobManager(const size_t jobId, const QString jobName)
{
	this->jobId = jobId;
	this->jobName = jobName;
}

QString JobManager::getJobName() const
{
	return this->jobName;
}

size_t JobManager::getJobId() const
{
	return this->jobId;
}
