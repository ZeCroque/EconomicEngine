#include "Observable.h"

bool Observable::contains(IObserver* observer)
{
	for (auto registeredObserver : this->_registeredObservers)
	{
		if (registeredObserver->equals(*observer))
		{
			return true;
		}
	}
	return false;
}

void Observable::addObserver(IObserver* observer)
{
	if (!this->contains(observer))
	{
		this->_registeredObservers.push_back(observer);
	}
}

void Observable::removeObserver(IObserver* observer)
{
	if (this->contains(observer))
	{
		this->_registeredObservers.remove(observer);
	}
}

std::list<IObserver*> Observable::getRegisteredObservers()
{
	return this->_registeredObservers;
}