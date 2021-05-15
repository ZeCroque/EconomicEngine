#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMainWindow>

class GraphManager final : public QCheckBox
{
Q_OBJECT
public:
	explicit GraphManager(QWidget* inParent);

    GraphManager(QWidget* inParent, size_t inItemId);

    [[nodiscard]] int getGraphIndex() const;
	void setGraphIndex(int inGraphIndex);
	[[nodiscard]] size_t getItemId() const;
	void setItemId(size_t inItemId);

	Q_PROPERTY(int gIndex MEMBER graphIndex READ getGraphIndex WRITE setGraphIndex)

private:
	int graphIndex;
	size_t itemId;
};

#endif //GRAPH_MANAGER_H