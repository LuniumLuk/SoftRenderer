#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"
#include "global.hpp"
#include "darray.hpp"

namespace Lurdr
{

// a discussion over size_t & long
// http://cplusplus.com/forum/beginner/87153/
class FrameBuffer
{
private:
    long   m_width;
    long   m_height;
    long   m_size;
    byte_t *m_color_buffer;
    float  *m_depth_buffer;
public:
    FrameBuffer();
    FrameBuffer(long width, long height);
    ~FrameBuffer();

    long getHeight() const;
    long getWidth() const;
    long getSize() const;
    byte_t* colorBuffer() const;
    float* depthBuffer() const;

    void clearColorBuffer(const RGBCOLOR & color) const;
    void clearDepthBuffer(const float & depth) const;
};

class ArrayBuffer
{
private:
    float  *m_buffer;
    size_t m_buffer_size;
    size_t m_data_count;
    DynamicArray< DynamicArray<float*> > m_data_pointers;
    DynamicArray<size_t> m_batch_sizes;

public:
    ArrayBuffer():
        m_buffer(nullptr),
        m_buffer_size(0),
        m_data_count(0) {}
    ~ArrayBuffer() {}

    void setBufferData(const size_t & size, float* data)
    {
        m_buffer_size = size;
        m_buffer = data;
    }

    void setDataPointers(
        const size_t & index, 
        const size_t & batch_size,
        const size_t & batch_step, 
        const size_t & offset )
    {
        m_data_pointers[index] = DynamicArray<float*>();
        m_batch_sizes[index] = batch_size;
        if (index >= m_data_count)
        {
            m_data_count = index + 1;
        }
        size_t pos = offset;
        while (pos < m_buffer_size)
        {
            m_data_pointers[index].push_back(&m_buffer[pos]);
            pos += batch_step;
        }
    }

    size_t getDataSize(const size_t & index)
    {
        return m_data_pointers[index].size();
    }

    size_t getDataCount() const
    {
        return m_data_count;
    }

    float* getData(const size_t & index, const size_t & pos)
    {
        // assert(pos < m_data_pointers[index].size());
        if (pos < m_data_pointers[index].size())
        {
            return m_data_pointers[index][pos];
        }
        else
        {
            return (float*)0;
        }
    }
};

class ElementBuffer
{
private:
    size_t *m_buffer;
    size_t m_buffer_size;
    DynamicArray<size_t*> m_data_pointer;

public:
    ElementBuffer():
        m_buffer(nullptr),
        m_buffer_size(0) {}
    ~ElementBuffer() {}

    void setBufferData(const size_t & size, size_t* data)
    {
        assert(size % 3 == 0);
        m_buffer_size = size;
        m_buffer = data;
        size_t pos = 0;
        while (pos < m_buffer_size)
        {
            m_data_pointer.push_back(&m_buffer[pos]);
            pos += 3;
        }
    }

    size_t getDataSize()
    {
        return m_data_pointer.size();
    }

    size_t* getData(const size_t & pos)
    {
        assert(pos < m_data_pointer.size());
        return m_data_pointer[pos];
    }
};

class VertexArray
{
private:
    ArrayBuffer     *m_data_array;
    ElementBuffer   *m_indicies_array;

public:
    VertexArray():
        m_data_array(nullptr),
        m_indicies_array(nullptr) {}
    ~VertexArray() {}

    void bindDataArray(ArrayBuffer * data_array)
    {
        m_data_array = data_array;
    }

    void bindIndiciesArray(ElementBuffer * indicies_array)
    {
        m_indicies_array = indicies_array;
    }

    size_t getTriangleCount() const
    {
        return m_indicies_array->getDataSize();
    }

    size_t getDataCount() const
    {
        return m_data_array->getDataCount();
    }

    float* getData(
        const size_t & triangle_index,
        const size_t & vertex_index,
        const size_t & data_index ) const
    {
        assert(vertex_index < 3);
        size_t data_pos = *(m_indicies_array->getData(triangle_index) + vertex_index) - 1;
        return m_data_array->getData(data_index, data_pos);
    }
};

}

#endif