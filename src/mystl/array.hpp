#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

namespace MySTL
{

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

}


#endif