#ifndef VECTOR_ARRAY_H
#define VECTOR_ARRAY_H

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

template <class T> class VectorArray
{
private:
	std::vector<size_t> keys;
	std::vector<std::shared_ptr<T>>* vectors;
public:
	VectorArray() : vectors(nullptr){}
	VectorArray(std::vector<size_t> keys) : keys(std::move(keys)), vectors(new std::vector<std::shared_ptr<T>>[this->keys.size()]) {}
	
	std::vector<std::shared_ptr<T>>& operator[](size_t key)
	{
		std::vector<std::shared_ptr<T>>* result = nullptr;
		for(unsigned int i=0; i<this->keys.size(); ++i)
		{
			if(this->keys[i] == key)
			{
				result = &vectors[i];
			}
		}
		if(result == nullptr)
		{
			result = new std::vector<std::shared_ptr<T>>();
		};
		return *result;
	}
};

#endif
