#include "../include/graphics.hpp"

using namespace Lurdr;

// note : https://stackoverflow.com/questions/31884589/linker-cant-find-function-definition-in-a-namespace
void Lurdr::drawPoint(const FrameBuffer & frame_buffer, Vector2 position, RGBColor color)
{
    size_t pos_x = position.x;
    size_t pos_y = position.y;
    assert(pos_x >= 0 && pos_x < frame_buffer.getWidth());
    assert(pos_y >= 0 && pos_y < frame_buffer.getHeight());

    byte_t *color_buffer = frame_buffer.colorBuffer();
    size_t buffer_pos = (frame_buffer.getWidth() * pos_y + pos_x) * 3;
    color_buffer[buffer_pos++] = color.R;
    color_buffer[buffer_pos++] = color.G;
    color_buffer[buffer_pos] = color.B;
}
// reference : https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Lurdr::drawLine(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, RGBColor color)
{
    long x1 = v1.x;
    long y1 = v1.y;
    long x2 = v2.x;
    long y2 = v2.y;
    assert(x1 >= 0 && x1 <= frame_buffer.getWidth());
    assert(y1 >= 0 && y1 <= frame_buffer.getHeight());
    assert(x2 >= 0 && x2 <= frame_buffer.getWidth());
    assert(y2 >= 0 && y2 <= frame_buffer.getHeight());

    long dx = abs(x2 - x1);
    long dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    long err = dx + dy; 
    long e2;

    byte_t *color_buffer = frame_buffer.colorBuffer();
    size_t buffer_pos = (y1 * frame_buffer.getWidth() + x1) * 3;
    size_t end_pos = (y2 * frame_buffer.getWidth() + x2) * 3;
    while (true)
    {
        color_buffer[buffer_pos] = color.R;
        color_buffer[buffer_pos + 1] = color.G;
        color_buffer[buffer_pos + 2] = color.B;

        if (buffer_pos == end_pos) break;

        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            buffer_pos += sx * 3;
        }
        if (e2 <= dx)
        {
            err += dx;
            buffer_pos += sy * frame_buffer.getWidth() * 3;
        }
    }
}

void Lurdr::drawTriangle(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color)
{

}
