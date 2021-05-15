#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <string>

#include "Clonable.h"

class Tradable;
class Uncountable;

class Behavior : public Clonable<Behavior>
{
public:
	Behavior();
	
	explicit Behavior(const std::string& inName);
	
	virtual void init(Uncountable* inOwningTool) = 0;
	
	[[nodiscard]] size_t getId() const;

protected:
	Uncountable* owningTool;
	size_t id;

};
#endif //BEHAVIOR_H
