#ifndef __DARRAY_HPP__
#define __DARRAY_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <utility>
#include "global.hpp"

namespace Lurdr
{

// Array implement basic functionalities as STL std::array
template <typename T, size_t S>
class Array
{
private:
    T m_data[S]; 
public:
    constexpr size_t size() const { return S; }

    T& operator[] (size_t index) { return m_data[index]; }
    const T& operator[] (size_t index) const { return m_data[index]; }

    T* data() { return m_data; }
    const T* data() const { return m_data; }
};

// DynamicArray implement basic functionalities as STL std::vector
template<typename T>
class DynamicArray
{
private:
    T       *m_array;
    size_t  m_size;
    size_t  m_capacity;

    size_t qsort_partition(long low, long high, bool (*cmp)(const void *, const void *));
    void qsort(long low, long high, bool (*cmp)(const void *, const void *));
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

    void sort(bool (*cmp)(const void *, const void *));
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
    assert(pos < m_size);
    return m_array[pos];
}
template<typename T>
T& DynamicArray<T>::operator[] (const size_t & pos)
{
    if (pos >= m_capacity)
    {
        m_capacity = pos + 1;
        T *new_array = new T[m_capacity];
        memset(new_array, 0, m_capacity * sizeof(T));
        memcpy(new_array, m_array, m_size * sizeof(T));
        delete[] m_array;
        m_array = new_array;
        m_size = m_capacity;
    }
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

// implement of quick sort
// reference : https://www.geeksforgeeks.org/cpp-program-for-quicksort/
template<typename T>
size_t DynamicArray<T>::qsort_partition(long low, long high, bool (*cmp)(const void *, const void *))
{
    T &pivot = m_array[high];
    long i = low;
    long j = high - 1;
    while (true)
    {
        while (!cmp(&m_array[i], &pivot) && i < high)
        {
            i++;
        }
        while (cmp(&m_array[j], &pivot) && j > low)
        {
            j--;
        }

        if (i >= j)
        {
            std::swap(m_array[i], m_array[high]);
            return i;
        }

        std::swap(m_array[i], m_array[j]);
    }
}
template<typename T>
void DynamicArray<T>::qsort(long low, long high, bool (*cmp)(const void *, const void *))
{
    if (low < high)
    {
        long pivot = qsort_partition(low, high, cmp);
        
        qsort(low, pivot - 1, cmp);
        qsort(pivot + 1, high, cmp);
    }
}

template<typename T>
void DynamicArray<T>::sort(bool (*cmp)(const void *, const void *))
{
    qsort(0, m_size - 1, cmp);
}

}

#endif
