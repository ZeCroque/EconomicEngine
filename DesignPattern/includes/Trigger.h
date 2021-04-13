#ifndef TRIGGER_H
#define TRIGGER_H

#include "Signal.h"
#include <any>
#include <functional>

template<class T> class Trigger final
{
public:
	Trigger() = default;
	Trigger(const Trigger& trigger) : value(trigger.value){}
	explicit  Trigger(const T& value) : value(value){}
	~Trigger() = default;

	Trigger& operator=(const T& newValue)
	{
		if(value != newValue)
		{
			value = newValue;
			notify();
		}
		return *this;
	}

	Trigger& operator=(const Trigger& trigger)
	{
		if(value != trigger.value)
		{
			value = trigger.value;
			notify();
		}

		return *this;
	}

	operator T() const
	{
		return value;
	}

	void setWithoutNotifying(const Trigger& trigger)
	{
		if(value != trigger.value)
		{
			value = trigger.value;
		}
	}

	void setWithoutNotifying(const T& newValue)
	{
		if(value != newValue)
		{
			value = newValue;
		}
	}

	void connect(const std::function<void(const std::any&)>& func) const
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		signal.connect(func);
	}

	void connect(const std::function<void(const T&)>& func) const
	{
		const auto funcAny = [func](const std::any& lhs)
		{
			const auto element = std::any_cast<T>(lhs);
			func(element);
		};
		signal.connect(funcAny);
	}
private:
	void notify()
	{
		signal(value);
	}
	
	T value;
	mutable Signal<const std::any&> signal;
};

#endif //TRIGGER_H

