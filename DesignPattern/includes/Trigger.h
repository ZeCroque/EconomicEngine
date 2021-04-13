#ifndef TRIGGER_H
#define TRIGGER_H

#include "Signal.h"
#include <any>
#include <functional>

using EventSignal = Signal<const std::any&>;

template<class T> class Trigger final
{
public:
	Trigger() = default;
	Trigger(const Trigger& trigger) : m_value(trigger.m_value){}
	explicit  Trigger(const T& value) : m_value(value){}
	~Trigger() = default;

	Trigger& operator=(const T& value)
	{
		if(m_value != value)
		{
			m_value = value;
			Notify();
		}
		return *this;
	}

	Trigger& operator=(const Trigger& trigger)
	{
		if(m_value != trigger.m_value)
		{
			m_value = trigger.m_value;
			Notify();
		}

		return *this;
	}

	operator T() const
	{
		return m_value;
	}

	void SetNoTrig(const Trigger& trigger)
	{
		if(m_value != trigger.m_value)
		{
			m_value = trigger.m_value;
		}
	}

	void SetNoTrig(const T& value)
	{
		if(m_value != trigger)
		{
			m_value = value;
		}
	}

	void Connect(const std::function<void(const std::any&)>& func) const
	{
		m_signal.Connect(func);
	}

	void ConnectTyped(const std::function<void(const T&)>& func) const
	{
		const auto funcAny = [func](const std::any& lhs)
		{
			const auto element = std::any_cast<T>(lhs);
			func(element);
		};
		m_signal.Connect(funcAny);
	}
private:
	void Notify()
	{
		m_signal(m_value);
	}
	
	T m_value;
	mutable EventSignal m_signal;
};

#endif //TRIGGER_H

