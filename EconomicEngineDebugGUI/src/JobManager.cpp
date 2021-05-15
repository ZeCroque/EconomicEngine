#include "JobManager.h"

JobManager::JobManager(const size_t inJobId, const QString& inJobName) : jobName(inJobName), jobId(inJobId)
{
}

QString JobManager::getJobName() const
{
	return jobName;
}

size_t JobManager::getJobId() const
{
	return jobId;
}
