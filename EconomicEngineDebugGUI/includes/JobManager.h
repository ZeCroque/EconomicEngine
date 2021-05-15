#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "QtWidgets/QLabel"
#include "QtCore/QString"

class JobManager final
{
public:
	JobManager(size_t inJobId, const QString& inJobName);

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

#endif //GRAPH_MANAGER_H
