#pragma once

#include <list>
#include "Observer.h"

class Observable
{
private:
	std::list<IObserver*> registeredObservers;

public:
	Observable() = default;
	Observable(const Observable&) = default;
	Observable(Observable&&) = default;
	Observable& operator=(const Observable&) = default;
	Observable& operator=(Observable&&) = default;
	virtual ~Observable() = default;
	
	
	void addObserver(IObserver* observer);
	void removeObserver(IObserver* observer);
	[[nodiscard]] std::list<IObserver*> getRegisteredObservers() const;
	virtual void notifyObservers();

private:
	bool markForDelete;
	
	bool contains(IObserver* observer);
};