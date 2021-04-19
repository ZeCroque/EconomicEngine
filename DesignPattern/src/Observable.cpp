#include "Observable.h"

bool Observable::contains(IObserver* const observer)
{
	for (auto* registeredObserver : registeredObservers)
	{
		if (registeredObserver == observer)
		{
			return true;
		}
	}
	return false;
}

void Observable::addObserver(IObserver* const observer)
{
	if (!contains(observer))
	{
		registeredObservers.emplace_back(observer);
	}
}

void Observable::removeObserver(IObserver* const observer)
{
	for (auto*& registeredObserver : registeredObservers)
	{
		if (registeredObserver == observer)
		{
			registeredObserver = nullptr;
			break;
		}
	}
}

std::list<IObserver*> Observable::getRegisteredObservers() const
{
	return registeredObservers;
}

void Observable::notifyObservers()
{
	for (auto* registeredObserver : registeredObservers)
	{
		if (registeredObserver != nullptr)
		{
			registeredObserver->notify(this);
		}
	}
}
