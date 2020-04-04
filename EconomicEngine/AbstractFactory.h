#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include <map>

template <class Key, class T> class AbstractFactory
{
	std::map<Key, T*> objectsMap;
public:

	void registerObject(Key key, T* obj)
	{
		if (objectsMap.find(key) == objectsMap.end())
		{
			objectsMap[key] = obj;
		}
	}

	T* createObject(const Key& key)
	{
		T* result = nullptr;
		auto it = objectsMap.find(key);
		if (it != objectsMap.end())
		{
			result = ((*it).second)->clone();
		}
		return result;
	}

protected:
	std::vector<Key> getKeys()
	{
		std::vector<Key> keys;
		for (auto item : objectsMap)
		{
			keys.emplace_back(item.first);
		}
		return keys;
	}

	T* getDefaultObject(size_t key)
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
