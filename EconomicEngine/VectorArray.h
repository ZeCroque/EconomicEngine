#ifndef VECTOR_ARRAY_H
#define VECTOR_ARRAY_H

#include <cassert>
#include <memory>
#include <vector>

template <class T> class VectorArray
{
private:
	std::vector<std::shared_ptr<T>>* vectors;
	std::vector<size_t>* keys;
public:
	VectorArray() : vectors(nullptr), keys(nullptr){}
	VectorArray(std::vector<size_t>* keys)
	{
		this->keys = keys;
		this->vectors = new std::vector<std::shared_ptr<T>>[this->keys->size()];
	}
	std::vector<std::shared_ptr<T>>& operator[](size_t key)
	{
		std::vector<std::shared_ptr<T>>* result = nullptr;
		for(unsigned int i=0; i<this->keys->size(); ++i)
		{
			if(this->keys->operator[](i) == key)
			{
				result = &vectors[i];
			}
		}
		assert(result != nullptr);
		return *result;
	}
};

#endif
