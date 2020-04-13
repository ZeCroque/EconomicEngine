#pragma once

#include <QtWidgets/QMainWindow>

class JobManager final
{
Q_OBJECT
public:
	JobManager(QWidget* parent)
	{
	}

	std::string getJobName() const;
	void setJobName(std::string jobName);

	size_t getJobId() const;
	void setJobId(size_t jobId);

	Q_PROPERTY(int gIndex MEMBER graphIndex READ getGraphIndex WRITE setGraphIndex)

private:
	std::string jobName;
	size_t jobId;
};
