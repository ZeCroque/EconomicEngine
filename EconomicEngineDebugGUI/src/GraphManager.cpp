#include "GraphManager.h"

GraphManager::GraphManager(QWidget* inParent) : QCheckBox(inParent), graphIndex(0), itemId(0)
{
}

GraphManager::GraphManager(QWidget *inParent, const size_t inItemId) : QCheckBox(inParent), graphIndex(0), itemId(inItemId)
{
}

int GraphManager::getGraphIndex() const
{
	return graphIndex;
}

void GraphManager::setGraphIndex(const int inGraphIndex)
{
	graphIndex = inGraphIndex;
}

size_t GraphManager::getItemId() const
{
	return itemId;
}

void GraphManager::setItemId(const size_t inItemId)
{
	itemId = inItemId;
}
