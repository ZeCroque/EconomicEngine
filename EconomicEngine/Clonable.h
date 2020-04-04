#ifndef CLONABLE_H
#define CLONABLE_H

template <class T> class Clonable
{
public:
	virtual T* clone() = 0;
};

#endif
