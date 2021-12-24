#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"
#include "global.hpp"

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

}

#endif