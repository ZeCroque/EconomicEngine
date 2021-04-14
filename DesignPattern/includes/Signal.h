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

    Signal(const Signal& signal)
    {
        id = signal.id;
    	for(auto&& [id, slot] : signal.slots)
    	{
    		slots[id] = slot;
    	}
    }


	void disconnect(const int& id) const
    {
	    slots.erase(id);
    }

	void disconnectAll() const
    {
	    slots.clear();
    	id = 0;
    }

    int connect(const std::function<void(Args...)>& slot) const
    {
	    slots.insert({++id, slot});
    	return id;
    }

	template<class T> int connect(T* instance, void(T::*func)(Args...)) const
    {
    	return connect([=](Args... args)
    	{
    		(instance->*func)(args...);
    	});
    }

	template<class T> int connect(T* instance, void(T::*func)(Args...) const) const
    {
    	return connect([=](Args... args)
    	{
    		(instance->*func)(args...);
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

#define slots Q_SLOTS;

#endif //SIGNAL_H