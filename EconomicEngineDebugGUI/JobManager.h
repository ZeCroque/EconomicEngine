#pragma once

#include <QLabel>
#include <QString>

class JobManager final
{
public:
	JobManager(const size_t jobId, const QString jobName);

	QString getJobName() const;
	size_t getJobId() const;

	QLabel* lbName;
	QLabel* lbNumber;
	QLabel* lbMoneyAverage;
	QLabel* lbFoodAverage;
	QLabel* lbBirth;
	QLabel* lbDead;

private:
	QString jobName;
	size_t jobId{};
};
