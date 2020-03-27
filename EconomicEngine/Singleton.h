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
		if (Singleton<T>::instance == nullptr)
		{
			Singleton<T>::instance = new T();
		}
		return Singleton<T>::instance;
	}

	static void destroyInstance()
	{
		delete Singleton<T>::instance;
	}
};

template <class T> T* Singleton<T>::instance = nullptr;


#endif
