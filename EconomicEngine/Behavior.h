#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "Clonable.h"
#include "Observer.h"

class Tradable;
class Trader;

class Behavior : public Clonable<Behavior>, public IObserver
{
protected:
	Tradable* item;
	Trader* owner;
	size_t typeId;
public:
	Behavior();
	virtual void init(Trader* owner, Tradable* item) = 0;
	[[nodiscard]] size_t getId() const;

};
#endif
