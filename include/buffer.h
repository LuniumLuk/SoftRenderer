#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.h"
#include "global.h"

namespace Lurdr
{

class FrameBuffer
{
private:
    size_t  m_width;
    size_t  m_height;
    byte_t *m_color_buffer;
    float   *m_depth_buffer;
public:
    FrameBuffer() {}
    FrameBuffer(size_t width, size_t height): m_width(width), m_height(height)
    {
        size_t buffer_size = m_width * m_height;
        m_color_buffer = new byte_t[buffer_size * 3];
        m_depth_buffer = new float[buffer_size];
    }
    ~FrameBuffer()
    {
        delete[] m_color_buffer;
        delete[] m_depth_buffer;
    }
};

}

#endif