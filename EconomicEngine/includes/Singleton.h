#ifndef SINGLETON_H
#define SINGLETON_H

template<class T> class Singleton
{
protected:
	static T* instance;
	Singleton() = default;
	virtual ~Singleton() = default;
public :
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator = (const Singleton&&) = delete;


	static T* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new T();
		}
		return instance;
	}

	static void destroyInstance()
	{
		delete instance;
	}
};

template <class T> T* Singleton<T>::instance = nullptr;


#endif
