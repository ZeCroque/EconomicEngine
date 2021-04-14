//
// Created by relin on 14/04/2021.
//

#include <MovableActor.h>

Actor* MovableActor::clone()
{
	return new MovableActor(*this);
}
