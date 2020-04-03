#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include <map>

template <class T> class AbstractFactory
{
    static std::map<int, T*> m_map;
public:

    static void Register(int key, T* obj);
    T* Create(const int & key);
};

template <class T> std::map<int, T*> AbstractFactory<T>::m_map = std::map<int, T*>();

template <class T> void AbstractFactory<T>::Register(int key, T* obj)
{
    if (m_map.find(key) == m_map.end())
    {
        m_map[key] = obj;
    }
}

template <class T> T* AbstractFactory<T>::Create(const int& key)
{
    T* tmp = nullptr;
    typename std::map<int, T*>::iterator it = m_map.find(key);

    if (it != m_map.end())
    {
        tmp = ((*it).second)->Clone();
    }

    return tmp;
}

#endif
