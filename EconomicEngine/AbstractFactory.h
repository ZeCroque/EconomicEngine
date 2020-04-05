#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include<vector>
#include <map>

template <class Key, class T> class AbstractFactory
{
protected:
	std::map<Key, T*> objectsMap;
	
public:

	void registerObject(Key key, T* obj)
	{
		if (objectsMap.find(key) == objectsMap.end())
		{
			objectsMap[key] = obj;
		}
	}

	[[nodiscard]] T* createObject(const Key& key) const
	{
		T* result = nullptr;
		auto it = objectsMap.find(key);
		if (it != objectsMap.end())
		{
			result = ((*it).second)->clone();
		}
		return result;
	}

	std::vector<Key> getKeys() const
	{
		std::vector<Key> keys;
		for (auto item : objectsMap)
		{
			keys.emplace_back(item.first);
		}
		return keys;
	}
	
protected:
	[[nodiscard]] T* getDefaultObject(size_t key) const
	{
		T* defaultObject = nullptr;
		auto it = objectsMap.find(key);
		if(it!=objectsMap.end())
		{	
			defaultObject = it->second;
		}
		return  defaultObject;
	}

};

#endif
