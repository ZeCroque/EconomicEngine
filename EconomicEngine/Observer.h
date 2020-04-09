#pragma once

class Observable;

class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void notify(Observable* sender) = 0;
};
