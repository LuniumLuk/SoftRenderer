#ifndef __DARRAY_HPP__
#define __DARRAY_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "global.hpp"

namespace Lurdr
{

// DynamicArray implement basic functionalities as STL std::vector
template<typename T>
class DynamicArray
{
private:
    T       *m_array;
    size_t  m_size;
    size_t  m_capacity;
public:
    DynamicArray(): m_array(NULL),
                    m_size(0),
                    m_capacity(0) {}
    DynamicArray(const size_t & reserve);
    DynamicArray(const DynamicArray & array);
    ~DynamicArray();

    void push_back(const T element);
    void pop_back();
    T back() const;
    T front() const;
    T at(const size_t & pos) const;
    T& operator[] (const size_t & pos);
    void clear();
    bool empty() const;
    size_t size() const;
    size_t capacity() const;
    T* data() const;
};

// note : classes with template hvae to implement their member functions within the header file
// see : https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
template<typename T>
DynamicArray<T>::DynamicArray(const size_t & reserve)
{
    m_size = reserve;
    m_capacity = reserve;
    m_array = new T[m_size];
}
template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray & array)
{
    m_size = array.m_size;
    m_capacity = array.m_capacity;
    memcpy(m_array, array.m_array, m_size);
}
template<typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] m_array;
}

template<typename T>
void DynamicArray<T>::push_back(const T element)
{
    if (m_capacity == 0)
    {
        m_capacity = 1;
        m_array = new T[m_capacity];
        m_array[m_size++] = element;
    }
    else if (m_size == m_capacity)
    {
        m_capacity *= 2;
        T *new_array = new T[m_capacity];
        memcpy(new_array, m_array, m_size * sizeof(T));
        delete[] m_array;
        m_array = new_array;
        m_array[m_size++] = element;
    }
    else
    {
        m_array[m_size++] = element;
    }
}

template<typename T>
void DynamicArray<T>::pop_back()
{
    assert(m_size > 0);
    m_size--;
}

template<typename T>
T DynamicArray<T>::back() const
{
    assert(m_size > 0);
    return m_array[m_size - 1];
}
template<typename T>
T DynamicArray<T>::front() const
{
    assert(m_size > 0);
    return m_array[0];
}
template<typename T>
T DynamicArray<T>::at(const size_t & pos) const
{   
    assert(pos >= 0 && pos < m_size);
    return m_array[pos];
}
template<typename T>
T& DynamicArray<T>::operator[] (const size_t & pos)
{
    assert(pos >= 0 && pos < m_size);
    return m_array[pos];
}
template<typename T>
void DynamicArray<T>::clear()
{
    m_size = 0;
    m_capacity = 0;

}
template<typename T>
bool DynamicArray<T>::empty() const
{
    return m_size == 0;
}
template<typename T>
size_t DynamicArray<T>::size() const
{
    return m_size;
}
template<typename T>
size_t DynamicArray<T>::capacity() const
{
    return m_capacity;
}
template<typename T>
T* DynamicArray<T>::data() const
{
    return m_array;
}

}

#endif