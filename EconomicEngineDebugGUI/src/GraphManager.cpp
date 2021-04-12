#include "GraphManager.h"

int GraphManager::getGraphIndex() const
{
	return graphIndex;
}

void GraphManager::setGraphIndex(const int newGraphIndex)
{
	graphIndex = newGraphIndex;
}

size_t GraphManager::getItemId() const
{
	return itemId;
}

void GraphManager::setItemId(const size_t newItemId)
{
	itemId = newItemId;
}

GraphManager::GraphManager(QWidget *parent, size_t itemId) : QCheckBox(parent), itemId(itemId) {}
