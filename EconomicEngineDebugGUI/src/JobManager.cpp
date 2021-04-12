#include "JobManager.h"

JobManager::JobManager(const size_t newJobId, const QString& newJobName)
{
	jobId = newJobId;
	jobName = newJobName;
}

QString JobManager::getJobName() const
{
	return jobName;
}

size_t JobManager::getJobId() const
{
	return jobId;
}
