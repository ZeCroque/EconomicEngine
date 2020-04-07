#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include<vector>
#include <map>

template <class Key, class T> class AbstractFactory
{
protected:
	std::map<Key, T*> defaultObjects;
	
public:

	virtual ~AbstractFactory()
	{
		for(auto defaultObject : defaultObjects)
		{
			delete defaultObject.second;
		}
	}
	
	void registerObject(Key key, T* obj)
	{
		if (defaultObjects.find(key) == defaultObjects.end())
		{
			defaultObjects[key] = obj;
		}
	}

	[[nodiscard]] T* createObject(const Key& key) const
	{
		T* result = nullptr;
		auto it = defaultObjects.find(key);
		if (it != defaultObjects.end())
		{
			result = ((*it).second)->clone();
		}
		return result;
	}

	[[nodiscard]] std::vector<Key> getKeys() const
	{
		std::vector<Key> keys;
		for (auto item : defaultObjects)
		{
			keys.emplace_back(item.first);
		}
		return keys;
	}
	
protected:
	[[nodiscard]] T* getDefaultObject(size_t key) const
	{
		T* defaultObject = nullptr;
		auto it = defaultObjects.find(key);
		if(it!=defaultObjects.end())
		{	
			defaultObject = it->second;
		}
		return  defaultObject;
	}
};

#endif
