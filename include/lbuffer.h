#ifndef __LBUFFER_H__
#define __LBUFFER_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lmath.h"
#include "lglobal.h"

namespace Lurdr {

class FrameBuffer {
private:
    size_t  m_width;
    size_t  m_height;
    uchar_t *m_color_buffer;
    float   *m_depth_buffer;
public:
    FrameBuffer() {}
    FrameBuffer(size_t width, size_t height): m_width(width), m_height(height) {
        size_t buffer_size = m_width * m_height;
        m_color_buffer = new uchar_t[buffer_size * 3];
        m_depth_buffer = new float[buffer_size];
    }
    ~FrameBuffer() {
        delete[] m_color_buffer;
        delete[] m_depth_buffer;
    }
};

}

#endif