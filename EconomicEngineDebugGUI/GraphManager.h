#pragma once

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMainWindow>

class GraphManager final : public QCheckBox
{
Q_OBJECT
public:
	GraphManager(QWidget* parent): QCheckBox(parent), graphIndex(0)
	{
	}

	int getGraphIndex() const;
	void setGraphIndex(int graphIndex);
	size_t getItemId() const;
	void setItemId(size_t itemId);

	Q_PROPERTY(int gIndex MEMBER graphIndex READ getGraphIndex WRITE setGraphIndex)

private:
	int graphIndex;
	size_t itemId;
};
