#pragma once

class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void notify() = 0;
};