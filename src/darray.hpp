#ifndef __DARRAY_HPP__
#define __DARRAY_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <utility>
#include "global.hpp"

namespace LuGL
{

// Array implement basic functionalities as STL std::array
template <typename T, size_t S>
class Array
{
private:
    T m_data[S]; 
public:
    size_t size() const { return S; }

    T& operator[] (size_t index) { return m_data[index]; }
    const T& operator[] (size_t index) const { return m_data[index]; }

    T* data() { return m_data; }
    const T* data() const { return m_data; }
};

typedef Array<long,3> vec3i;

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
    void reserve(size_t size);
    T back() const;
    T front() const;
    T at(const size_t & pos) const;
    T& operator[] (const size_t & pos);
    const T& operator[] (const size_t & pos) const;
    void clear();
    bool empty() const;
    size_t size() const;
    size_t capacity() const;
    T* data() const;

    void sort(bool (*cmp)(const T &, const T &));
};

// note : classes with template have to implement their member functions within the header file
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
    for (size_t i = 0; i < m_size; i++)
    {
        m_array[i] = std::move(array.m_array[i]);
    }
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
        for (size_t i = 0; i < m_size; i++)
        {
            new_array[i] = m_array[i];
        }
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
void DynamicArray<T>::reserve(size_t size) {
    if (m_capacity >= size) return;

    m_capacity = size;
    T *new_array = new T[m_capacity];
    for (size_t i = 0; i < m_size; i++)
    {
        new_array[i] = m_array[i];
    }
    delete[] m_array;
    m_array = new_array;
}

template<typename T>
void DynamicArray<T>::pop_back()
{
    assert(m_size > 0);
    m_size--;
    m_array[m_size].~T();
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
    assert(pos < m_size);
    return m_array[pos];
}
template<typename T>
T& DynamicArray<T>::operator[] (const size_t & pos)
{
    assert(pos < m_capacity);
    // if (pos >= m_capacity)
    // {
    //     m_capacity = pos + 1;
    //     T *new_array = new T[m_capacity];
    //     for (size_t i = 0; i < m_size; i++)
    //     {
    //         new_array[i] = m_array[i];
    //     }
    //     delete[] m_array;
    //     m_array = new_array;
    //     m_size = m_capacity;
    // }
    return m_array[pos];
}
template<typename T>
const T& DynamicArray<T>::operator[] (const size_t & pos) const
{
    assert(pos < m_capacity);
    return m_array[pos];
}
template<typename T>
void DynamicArray<T>::clear()
{
    for (size_t i = 0; i < m_size; i++)
    {
        m_array[i].~T();
    }
    m_size = 0;
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

template<typename T>
void DynamicArray<T>::sort(bool(*cmp)(const T &, const T &))
{
    qsort(m_array, 0, m_size - 1, cmp);
}

}

#endif
