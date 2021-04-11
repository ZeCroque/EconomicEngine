#include "GraphManager.h"

int GraphManager::getGraphIndex() const
{
	return this->graphIndex;
}

void GraphManager::setGraphIndex(const int graphIndex)
{
	this->graphIndex = graphIndex;
}

size_t GraphManager::getItemId() const
{
	return this->itemId;
}

void GraphManager::setItemId(const size_t itemId)
{
	this->itemId = itemId;
}
