#pragma once

#include <QString>

class JobManager final
{
public:
	JobManager(const size_t jobId, const QString jobName);

	QString getJobName() const;
	size_t getJobId() const;

private:
	QString jobName;
	size_t jobId{};
};
