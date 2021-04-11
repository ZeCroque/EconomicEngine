#pragma once

#include <list>
#include "Observer.h"

class Observable
{
private:
	std::list<IObserver*> registeredObservers;

public:	
	void addObserver(IObserver* observer);
	void removeObserver(IObserver* observer);
	[[nodiscard]] std::list<IObserver*> getRegisteredObservers() const;
	virtual void notifyObservers();

private:	
	bool contains(IObserver* observer);
};