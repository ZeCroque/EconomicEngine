#include "StaticActor.h"

Actor* StaticActor::clone()
{
	return new StaticActor(*this);
}
