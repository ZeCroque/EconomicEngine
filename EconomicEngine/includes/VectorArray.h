#ifndef VECTOR_ARRAY_H
#define VECTOR_ARRAY_H

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
	explicit VectorArray(std::vector<size_t> inKeys) : keys(std::move(inKeys)), vectors(new std::vector<std::shared_ptr<T>>[keys.size()]) {}
	
	std::vector<std::shared_ptr<T>>& operator[](const size_t key)
	{
		std::vector<std::shared_ptr<T>>* result = nullptr;
		for(size_t i = 0; i < keys.size(); ++i)
		{
			if(keys[i] == key)
			{
				result = &vectors[i];
			}
		}
		if(!result)
		{
			result = new std::vector<std::shared_ptr<T>>();
		}
		return *result;
	}

	std::vector<std::shared_ptr<T>>& operator[](const size_t key) const
	{
		std::vector<std::shared_ptr<T>>* result = nullptr;
		for (size_t i = 0; i < keys.size(); ++i)
		{
			if (keys[i] == key)
			{
				result = &vectors[i];
			}
		}
		if (!result)
		{
			result = new std::vector<std::shared_ptr<T>>();
		}
		return *result;
	}
};

#endif //VECTOR_ARRAY_H
