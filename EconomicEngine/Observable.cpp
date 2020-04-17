#include "Observable.h"

bool Observable::contains(IObserver* const observer)
{
	for (auto* registeredObserver : this->registeredObservers)
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
	if (!this->contains(observer))
	{
		this->registeredObservers.emplace_back(observer);
	}
}

void Observable::removeObserver(IObserver* const observer)
{
	for (auto* registeredObserver : registeredObservers)
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
	return this->registeredObservers;
}

void Observable::notifyObservers()
{
	for (auto registeredObserver : this->registeredObservers)
	{
		if (registeredObserver != nullptr)
		{
			registeredObserver->notify(this);
		}
	}
}
