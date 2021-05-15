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
	
	void registerObject(Key inKey, T* inObject)
	{
		if (defaultObjects.find(inKey) == defaultObjects.end())
		{
			defaultObjects[inKey] = inObject;
		}
	}

	[[nodiscard]] T* createObject(const Key& inKey) const
	{
		T* result = nullptr;
		if (auto it = defaultObjects.find(inKey); it != defaultObjects.end())
		{
			result = ((*it).second)->clone();
		}
		return result;
	}

	[[nodiscard]] std::vector<Key> getKeys() const
	{
		std::vector<Key> keys;
		for (const auto& item : defaultObjects)
		{
			keys.emplace_back(item.first);
		}
		return keys;
	}
	
	[[nodiscard]] T* getDefaultObject(Key inKey) const
	{
		T* defaultObject = nullptr;
		if(auto it = defaultObjects.find(inKey); it != defaultObjects.end())
		{	
			defaultObject = it->second;
		}
		return  defaultObject;
	}
};

#endif
