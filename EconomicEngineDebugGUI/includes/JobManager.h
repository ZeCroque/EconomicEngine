#pragma once

#include "QtWidgets/QLabel"
#include "QtCore/QString"

class JobManager final
{
public:
	JobManager(size_t jobId, const QString& jobName);

	[[nodiscard]] QString getJobName() const;
	[[nodiscard]] size_t getJobId() const;

	QLabel* lbName{};
	QLabel* lbNumber{};
	QLabel* lbMoneyAverage{};
	QLabel* lbFoodAverage{};
	QLabel* lbBirth{};
	QLabel* lbDead{};

private:
	QString jobName;
	size_t jobId{};
};