#ifndef SIGNAL_H
#define SIGNAL_H

#include <atomic>
#include <map>
#include <functional>

#undef slots

template<class... Args> using Slot = std::function<void(Args...)>;

template<class... Args> class Signal final
{
public:
    Signal() : id(0){}

    ~Signal()
    {
        disconnectAll();
    }

    Signal(const Signal& inSignal)
    {
    	id = inSignal.id.load();
    	for(auto&& [id, slot] : inSignal.slots)
    	{
    		slots[id] = slot;
    	}
    }


	void disconnect(const int& inId) const
    {
	    slots.erase(inId);
    }

	void disconnectAll() const
    {
	    slots.clear();
    	id = 0;
    }

    int connect(const std::function<void(Args...)>& inSlot) const
    {
	    slots.insert({++id, inSlot});
    	return id;
    }

	template<class T> int connect(T* inInstance, void(T::*func)(Args...)) const
    {
    	return connect([=](Args... args)
    	{
    		(inInstance->*func)(args...);
    	});
    }

	template<class T> int connect(T* inInstance, void(T::*func)(Args...) const) const
    {
    	return connect([=](Args... args)
    	{
    		(inInstance->*func)(args...);
    	});
    }

	void operator()(Args... args) const
	{
		for(auto slot : slots)
		{
			slot.second(args...);
		}
	}
	
private:
    mutable std::map<int, Slot<Args...>> slots;
    mutable std::atomic<int> id;
};

#endif //SIGNAL_H