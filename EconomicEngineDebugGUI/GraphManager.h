#pragma once

#include <QCheckBox>
#include <QtWidgets/QMainWindow>
#include <utility>

class GraphManager final : public QCheckBox
{
Q_OBJECT
public:
	GraphManager(QWidget* parent): QCheckBox(parent), graphIndex(0)
	{
	}

	int getGraphIndex() const;
	void setGraphIndex(int graphIndex);

	Q_PROPERTY(int gIndex MEMBER graphIndex READ getGraphIndex WRITE setGraphIndex)
	
private:
	int graphIndex;

};
