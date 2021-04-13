#ifndef SIGNAL_H
#define SIGNAL_H

#include <atomic>
#include <map>
#include <functional>

template<class... Args> class Signal final
{
public:
    Signal() : m_currentID(0){}

    ~Signal()
    {
        DisconnectAll();
    }

    Signal(const Signal& signal)
    {
        m_currentID = signal.m_currentID;
    	for(auto&& [id, slot] : signal.m_slots)
    	{
    		m_slots[id] = slot;
    	}
    }


	void Disconnect(const int& id) const
    {
	    m_slots.erase(id);
    }

	void DisconnectAll() const
    {
	    m_slots.clear();
    	m_currentID = 0;
    }

    int Connect(const std::function<void(Args...)>& slot) const
    {
	    m_slots.insert({++m_currentID, slot});
    	return m_currentID;
    }

	template<class T> int Connect(T* pInstance, void(T::*func)(Args...)) const
    {
    	return Connect([=](Args... args)
    	{
    		(pInstance->*func)(args...);
    	});
    }

	template<class T> int Connect(T* pInstance, void(T::*func)(Args...) const) const
    {
    	return Connect([=](Args... args)
    	{
    		(pInstance->*func)(args...);
    	});
    }

	void operator()(Args... args)
	{
		for(auto slot : m_slots)
		{
			slot.second(args...);
		}
	}
	
private:
    mutable std::map<int, std::function<void(Args...)> > m_slots;
    mutable std::atomic<int> m_currentID;
};

#endif //SIGNAL_H