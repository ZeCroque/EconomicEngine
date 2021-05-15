#ifndef SINGLETON_H
#define SINGLETON_H
#include <mutex>

template<class T> class Singleton
{
public:
    virtual ~Singleton()
    {
        delete instance;
        instance = nullptr;
    }
    static T* getInstance()
    {
        std::call_once(onceFlag, []()
        {
            instance = new T;
        });
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
protected:
    Singleton() = default;
private:
    static T* instance;
    static std::once_flag onceFlag; 
};

template <class T> T* Singleton<T>::instance = nullptr;
template<class T> std::once_flag Singleton<T>::onceFlag;

#endif //SINGLETON_H