#ifndef VECTOR_ARRAY_H
#define VECTOR_ARRAY_H

#include <vector>

template <class T> class VectorArray
{
private:
	std::vector<std::reference_wrapper<T>>* vectors;
	std::vector<size_t>* keys;
public:
	VectorArray() : vectors(nullptr), keys(nullptr){}
	VectorArray(std::vector<size_t>* keys)
	{
		this->keys = keys;
		this->vectors = new std::vector<std::reference_wrapper<T>>[this->keys->size()];
	}
	std::vector<std::reference_wrapper<T>>* operator[](size_t key)
	{
		for(unsigned int i=0; i<this->keys->size(); ++i)
		{
			if(this->keys->operator[](i) == key)
			{
				return &vectors[i];
			}
		}
		return nullptr;
	}
};

#endif
