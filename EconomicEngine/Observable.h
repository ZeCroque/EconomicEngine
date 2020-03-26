#pragma once

#include <list>
#include "Observer.h"

class Observable
{
private:
	std::list<IObserver*> _registeredObservers;

public:
	Observable() = default;
	~Observable() = default;
	void addObserver(IObserver* observer);
	void removeObserver(IObserver* observer);
	std::list<IObserver*> getRegisteredObservers();
	virtual void notifyObservers() = 0;

private:
	bool contains(IObserver* observer);
};