#include "buffer.hpp"

using namespace Lurdr;

FrameBuffer::FrameBuffer(): m_width(0), m_height(0), m_size(0)
{
    m_color_buffer = nullptr;
    m_depth_buffer = nullptr;
}
FrameBuffer::FrameBuffer(long width, long height): m_width(width), m_height(height)
{
    m_size = m_width * m_height;
    long buffer_size = m_width * m_height;
    m_color_buffer = new byte_t[buffer_size * 3];
    m_depth_buffer = new float[buffer_size];
}

FrameBuffer::~FrameBuffer()
{
    delete[] m_color_buffer;
    delete[] m_depth_buffer;
}

long FrameBuffer::getHeight() const
{
    return m_height;
}

long FrameBuffer::getWidth() const
{
    return m_width;
}

long FrameBuffer::getSize() const
{
    return m_size;
}

byte_t* FrameBuffer::colorBuffer() const
{
    return m_color_buffer;
}

float* FrameBuffer::depthBuffer() const
{
    return m_depth_buffer;
}

void FrameBuffer::clearColorBuffer(const rgb & color) const
{
    size_t temp_buffer_size = 3 * sizeof(byte_t);
    byte_t temp_buffer[3] = { 
        (byte_t)FTOD(color.r * 255), 
        (byte_t)FTOD(color.g * 255),
        (byte_t)FTOD(color.b * 255) };
    byte_t *color_buffer_ptr = m_color_buffer;
    for (long i = 0; i < m_size; i++)
    {
        memcpy(color_buffer_ptr, temp_buffer, temp_buffer_size);
        color_buffer_ptr += 3;
    }
}

void FrameBuffer::clearColorBuffer(const RGBCOLOR & color) const
{
    size_t temp_buffer_size = 3 * sizeof(byte_t);
    byte_t temp_buffer[3] = { color.R, color.G, color.B };
    byte_t *color_buffer_ptr = m_color_buffer;
    for (long i = 0; i < m_size; i++)
    {
        memcpy(color_buffer_ptr, temp_buffer, temp_buffer_size);
        color_buffer_ptr += 3;
    }
}

void FrameBuffer::clearDepthBuffer(const float & depth) const
{
    for (long i = 0; i < m_size; i++)
    {
        m_depth_buffer[i] = depth;
    }
}
