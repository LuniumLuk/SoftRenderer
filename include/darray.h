#ifndef __DARRAY_H__
#define __DARRAY_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "global.h"

namespace Lurdr {

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
    ~DynamicArray()
    {
        delete[] m_array;
    }

    void push_back(const T element);
    void pop_back();
    T back() const;
    T front() const;
    T at(const size_t & pos) const;
    T& operator[] (const size_t & pos);
    void clear();
    bool empty() const;
};

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

}

#endif