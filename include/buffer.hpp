#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"
#include "global.hpp"

namespace Lurdr
{

class FrameBuffer
{
private:
    long   m_width;
    long   m_height;
    byte_t *m_color_buffer;
    float  *m_depth_buffer;
public:
    FrameBuffer() {}
    FrameBuffer(long width, long height): m_width(width), m_height(height)
    {
        long buffer_size = m_width * m_height;
        m_color_buffer = new byte_t[buffer_size * 3];
        m_depth_buffer = new float[buffer_size];
    }
    ~FrameBuffer()
    {
        delete[] m_color_buffer;
        delete[] m_depth_buffer;
    }

    long getHeight() const
    {
        return m_height;
    }
    long getWidth() const
    {
        return m_width;
    }
    byte_t* colorBuffer() const
    {
        return m_color_buffer;
    }
    float* depthBuffer() const
    {
        return m_depth_buffer;
    }
};

}

#endif