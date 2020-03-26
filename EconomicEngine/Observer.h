#pragma once

class IObserver
{
public:
	virtual void notify() = 0;
	virtual bool equals(IObserver& observer) = 0;
};