#include "rasterizer.hpp"

using namespace Lurdr;

// note : https://stackoverflow.com/questions/31884589/linker-cant-find-function-definition-in-a-namespace
void Lurdr::drawPoint(const FrameBuffer & frame_buffer, Vector2 position, RGBColor color)
{
    long pos_x = position.x;
    long pos_y = position.y;
    assert(pos_x >= 0 && pos_x < frame_buffer.getWidth());
    assert(pos_y >= 0 && pos_y < frame_buffer.getHeight());

    drawPixel(frame_buffer, pos_x, pos_y, color);
}

void Lurdr::drawPixel(const FrameBuffer & frame_buffer, const long & x, const long & y, const RGBColor & color)
{
    byte_t *color_buffer = frame_buffer.colorBuffer();
    long buffer_pos = (frame_buffer.getWidth() * y + x) * 3;
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
    bool x1_valid = x1 >= 0 && x1 <= frame_buffer.getWidth();
    bool y1_valid = y1 >= 0 && y1 <= frame_buffer.getHeight();
    bool x2_valid = x2 >= 0 && x2 <= frame_buffer.getWidth();
    bool y2_valid = y2 >= 0 && y2 <= frame_buffer.getHeight();

    if (!(x1_valid && y1_valid && x2_valid && y2_valid))
    {
        return;
    }

    long dx = abs(x2 - x1);
    long dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    long err = dx + dy; 
    long e2;

    while (true)
    {
        drawPixel(frame_buffer, x1, y1, color);

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void Lurdr::drawScanLine(
    const FrameBuffer & frame_buffer,
    const long & x1,
    const long & x2,
    const long & y,
    const RGBColor & color )
{
    byte_t rgb_buffer[3] = { color.R, color.G, color.B };
    size_t rgb_buffer_size = 3 * sizeof(byte_t);
    byte_t *buffer_ptr = frame_buffer.colorBuffer() + (frame_buffer.getWidth() * y + x1) * 3;
    for (long x = x1; x <= x2; x++)
    {
        memcpy(buffer_ptr, rgb_buffer, rgb_buffer_size);
        buffer_ptr += rgb_buffer_size;
    }
}

void Lurdr::drawScanLine(
    const FrameBuffer & frame_buffer,
    const long & x1,
    const long & x2,
    const long & y,
    const Vector3 & color1,
    const Vector3 & color2 )
{
    float r = color1.x;
    float g = color1.y;
    float b = color1.z;
    float dr = (color2.x - color1.x) / (x2 - x1);
    float dg = (color2.y - color1.y) / (x2 - x1);
    float db = (color2.z - color1.z) / (x2 - x1);
    byte_t *color_buffer = frame_buffer.colorBuffer();
    long buffer_pos = (frame_buffer.getWidth() * y + x1) * 3;
    for (long x = x1; x <= x2; x++)
    {
        r += dr;
        g += dg;
        b += db;
        color_buffer[buffer_pos++] = r + 0.5f;
        color_buffer[buffer_pos++] = g + 0.5f;
        color_buffer[buffer_pos++] = b + 0.5f;
    }
}

// reference : https://joshbeam.com/articles/triangle_rasterization/
void Lurdr::drawTriangle(
    const FrameBuffer & frame_buffer,
    const Vector2 & v1,
    const Vector2 & v2,
    const Vector2 & v3,
    const RGBColor & color )
{   
    // 1000 ~= 114ms
    // standardTriangleRasterization(frame_buffer, v1, v2, v3, color);
    // 1000 ~= 1620ms
    // barycentricTriangleRasterization(frame_buffer, v1, v2, v3, color);
    // 1000 ~= 118ms
    // bresenhamTriangleRasterization(frame_buffer, v1, v2, v3, color);
    // 1000 ~= 120ms
    improvedBresenhamTriangleRasterization(frame_buffer, v1, v2, v3, color);
}
void Lurdr::drawTriangle(
    const FrameBuffer & frame_buffer,
    const Vector2 & v1,
    const Vector2 & v2,
    const Vector2 & v3,
    const RGBColor & color1,
    const RGBColor & color2,
    const RGBColor & color3 )
{
    // 1000 ~= 330ms
    standardTriangleRasterization(frame_buffer, v1, v2, v3, color1, color2, color3);
}

void Lurdr::standardTriangleRasterization(
    const FrameBuffer & frame_buffer,
    const Vector2 & v1,
    const Vector2 & v2,
    const Vector2 & v3,
    const RGBColor & color )
{
    long x1 = v1.x;
    long y1 = v1.y;
    long x2 = v2.x;
    long y2 = v2.y;
    long x3 = v3.x;
    long y3 = v3.y;
    
    sortVerticesByY(x1, y1, x2, y2, x3, y3);
    if (y1 == y2)
    {
        standardFillFlatTriangle(frame_buffer, y1, y3, x1, x2, x3, color);
    }
    else if (y2 == y3)
    {
        standardFillFlatTriangle(frame_buffer, y2, y1, x2, x3, x1, color);
    }
    else
    {
        /**
         * triangle vertices:
         *      [x3,y3]
         * [x2,y2]   [x4,y4]
         *               [x1,y1]
         */
        float x4 = x1 + (float)(x3 - x1) * (float)(y2 - y1) / (float)(y3 - y1);
        standardFillFlatTriangle(frame_buffer, y2, y3, x2, x4, x3, color);
        standardFillFlatTriangle(frame_buffer, y2, y1, x2, x4, x1, color);
    }
}

void Lurdr::standardTriangleRasterization(
    const FrameBuffer & frame_buffer,
    const Vector2 & v1,
    const Vector2 & v2,
    const Vector2 & v3,
    const RGBColor & color1,
    const RGBColor & color2,
    const RGBColor & color3 )
{
    long x1 = v1.x;
    long y1 = v1.y;
    long x2 = v2.x;
    long y2 = v2.y;
    long x3 = v3.x;
    long y3 = v3.y;
    Vector3 color_v1(color1.R, color1.G, color1.B); 
    Vector3 color_v2(color2.R, color2.G, color2.B); 
    Vector3 color_v3(color3.R, color3.G, color3.B); 
    if (y1 > y2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
        swap(color_v1, color_v2);
    }
    if (y2 > y3)
    {
        std::swap(x2, x3);
        std::swap(y2, y3);
        swap(color_v2, color_v3);
        if (y1 > y2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
            swap(color_v1, color_v2);
        }
    }
    
    if (y1 == y2)
    {
        standardFillFlatTriangle(frame_buffer, y1, y3, x1, x2, x3, color_v1, color_v2, color_v3);
    }
    else if (y2 == y3)
    {
        standardFillFlatTriangle(frame_buffer, y2, y1, x2, x3, x1, color_v2, color_v3, color_v1);
    }
    else
    {
        float x4 = x1 + (float)(x3 - x1) * (float)(y2 - y1) / (float)(y3 - y1);
        Vector3 color_v4 = color_v1 + (color_v3 - color_v1) * (float)(y2 - y1) / (float)(y3 - y1);
        standardFillFlatTriangle(frame_buffer, y2, y3, x2, x4, x3, color_v2, color_v4, color_v3);
        standardFillFlatTriangle(frame_buffer, y2, y1, x2, x4, x1, color_v2, color_v4, color_v1);
    }
}

/**
 * triangle vertices:
 *       [x3,y3]
 * [x1,y12]   [x2,y12]
 * 
 * note : x1 and x2 accept float input for accuracy
 */
void Lurdr::standardFillFlatTriangle(
    const FrameBuffer & frame_buffer,
    const long & y12,
    long  y3,
    float x1,
    float x2,
    const long & x3,
    const RGBColor & color)
{
    assert(x1 >= 0 && x1 <= frame_buffer.getWidth());
    assert(x2 >= 0 && x2 <= frame_buffer.getWidth());
    assert(x3 >= 0 && x3 <= frame_buffer.getWidth());
    assert(y12 >= 0 && y12 <= frame_buffer.getHeight());
    assert(y3 >= 0 && y3 <= frame_buffer.getHeight());

    if (x1 > x2)
    {
        std::swap(x1, x2);
    }
    long dy = y3 > y12 ? 1 : -1;
    y3 += dy;
    float dx1 = (float)(x3 - x1) / (y3 - y12);
    float dx2 = (float)(x3 - x2) / (y3 - y12);
    if (dy < 0)
    {
        dx1 = -dx1;
        dx2 = -dx2;
    }
    for (long y = y12; y != y3; y += dy)
    {
        drawScanLine(frame_buffer, x1, x2, y, color);
        x1 += dx1;
        x2 += dx2;
    }
}

void Lurdr::standardFillFlatTriangle(
    const FrameBuffer & frame_buffer,
    const long & y12,
    long  y3,
    float x1,
    float x2,
    const long & x3,
    Vector3 color1,
    Vector3 color2,
    const Vector3 & color3 )
{
    assert(x1 >= 0 && x1 <= frame_buffer.getWidth());
    assert(x2 >= 0 && x2 <= frame_buffer.getWidth());
    assert(x3 >= 0 && x3 <= frame_buffer.getWidth());
    assert(y12 >= 0 && y12 <= frame_buffer.getHeight());
    assert(y3 >= 0 && y3 <= frame_buffer.getHeight());

    if (x1 > x2)
    {
        std::swap(x1, x2);
        swap(color1, color2);
    }
    long dy = y3 > y12 ? 1 : -1;
    y3 += dy;
    x1 += 0.5f; // for type cast
    x2 += 0.5f;
    float dx1 = (float)(x3 - x1) / (y3 - y12);
    float dx2 = (float)(x3 - x2) / (y3 - y12);
    Vector3 dcolor1 = (color3 - color1) / (y3 - y12);
    Vector3 dcolor2 = (color3 - color2) / (y3 - y12);
    if (dy < 0)
    {
        dx1 = -dx1;
        dx2 = -dx2;
        dcolor1 = -dcolor1;
        dcolor2 = -dcolor2;
    }
    for (long y = y12; y != y3; y += dy)
    {
        drawScanLine(frame_buffer, x1, x2, y, color1, color2);
        x1 += dx1;
        x2 += dx2;
        color1 += dcolor1;
        color2 += dcolor2;
    }
}

// reference : http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo3
// quote : "Unfortunately, the simplicity of this algorithm comes at the price of low execution speed compared to the other two approaches."
void Lurdr::barycentricTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color)
{
    long x_max = max(v1.x, max(v2.x, v3.x));
    long x_min = min(v1.x, min(v2.x, v3.x));
    long y_max = max(v1.y, max(v2.y, v3.y));
    long y_min = min(v1.y, min(v2.y, v3.y));

    Vector2 vs1 = v2 - v1;
    Vector2 vs2 = v3 - v1;
    float dot_product = vs1.cross(vs2);

    for (long x = x_min; x <= x_max; x++)
    {
        for (long y = y_min; y <= y_max; y++)
        {
            Vector2 q(x - v1.x, y - v1.y);

            float s = q.cross(vs2) / dot_product;
            float t = vs1.cross(q) / dot_product;

            if (s >= 0 && t >= 0 && s + t <= 1)
            {
                drawPixel(frame_buffer, x, y, color);
            }
        }
    }
}

void Lurdr::sortVerticesByY(long & x1, long & y1, long & x2, long & y2, long & x3, long & y3)
{
    if (y1 > y2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    if (y2 > y3)
    {
        std::swap(x2, x3);
        std::swap(y2, y3);
        if (y1 > y2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
    }
}

void Lurdr::bresenhamTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color)
{
    long x1 = v1.x;
    long y1 = v1.y;
    long x2 = v2.x;
    long y2 = v2.y;
    long x3 = v3.x;
    long y3 = v3.y;

    sortVerticesByY(x1, y1, x2, y2, x3, y3);
    if (y1 == y2)
    {
        bresenhamFillFlatTriangle(frame_buffer, y1, y3, x1, x2, x3, color);
    }
    else if (y2 == y3)
    {
        bresenhamFillFlatTriangle(frame_buffer, y2, y1, x2, x3, x1, color);
    }
    else
    {
        /**
         * triangle vertices:
         *      [x3,y3]
         * [x2,y2]   [x4,y4]
         *               [x1,y1]
         */
        long x4 = x1 + (float)(x3 - x1) * (float)(y2 - y1) / (float)(y3 - y1) + 1.0f - EPSILON;
        bresenhamFillFlatTriangle(frame_buffer, y2, y3, x2, x4, x3, color);
        bresenhamFillFlatTriangle(frame_buffer, y2, y1, x2, x4, x1, color);
    }
}

void Lurdr::bresenhamFillFlatTriangle(const FrameBuffer & frame_buffer, long y12, long y3, long x1, long x2, long x3, RGBColor color)
{
    assert(x1 >= 0 && x1 <= frame_buffer.getWidth());
    assert(x2 >= 0 && x2 <= frame_buffer.getWidth());
    assert(x3 >= 0 && x3 <= frame_buffer.getWidth());
    assert(y12 >= 0 && y12 <= frame_buffer.getHeight());
    assert(y3 >= 0 && y3 <= frame_buffer.getHeight());

    if (x1 > x2)
    {
        std::swap(x1, x2);
    }
    long dx1 = abs(x3 - x1);
    long dx2 = abs(x3 - x2);
    long dy = -abs(y3 - y12);
    int sx1 = x1 < x3 ? 1 : -1;
    int sx2 = x2 < x3 ? 1 : -1;
    int sy = y12 < y3 ? 1 : -1;
    long err1 = dx1 + dy; 
    long err2 = dx2 + dy; 
    long e21;
    long e22;

    bool need_to_draw = true;
    while (true)
    {
        if (need_to_draw)
        {
            drawScanLine(frame_buffer, x1, x2, y12, color);
            need_to_draw = false;
        }

        if (x1 == x3 && x2 == x3 && y12 == y3) break;

        e21 = 2 * err1;
        e22 = 2 * err2;
        if (e21 >= dy)
        {
            err1 += dy;
            x1 += sx1;
        }
        if (e22 >= dy)
        {
            err2 += dy;
            x2 += sx2;
        }
        if (e21 <= dx1 && e22 <= dx2)
        {
            err1 += dx1;
            err2 += dx2;
            y12 += sy;
            need_to_draw = true;
        }
    }
}

void Lurdr::improvedBresenhamTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color)
{
    long x1 = v1.x;
    long y1 = v1.y;
    long x2 = v2.x;
    long y2 = v2.y;
    long x3 = v3.x;
    long y3 = v3.y;

    sortVerticesByY(x1, y1, x2, y2, x3, y3);
    long *x_left = new long[y3 - y1];
    long *x_right = new long[y3 - y1];
    memset(x_left, -1, (y3 - y1) * sizeof(long));
    memset(x_right, -1, (y3 - y1) * sizeof(long));

    fillTriangleTable(x_left, x_right, x1, y1, x2, y2, y1);
    fillTriangleTable(x_left, x_right, x1, y1, x3, y3, y1);
    fillTriangleTable(x_left, x_right, x2, y2, x3, y3, y1);

    drawLine(frame_buffer, v1, v2, color);
    drawLine(frame_buffer, v1, v3, color);
    drawLine(frame_buffer, v2, v3, color);

    for (int i = y1; i < y3; i++)
    {
        drawScanLine(frame_buffer, x_left[i-y1], x_right[i-y1], i, color);
    }

    delete[] x_left;
    delete[] x_right;
}

void Lurdr::fillTriangleTable(long * x_left, long * x_right, long x1, long y1, long x2, long y2, long y_min)
{
    y1 -= y_min;
    y2 -= y_min;
    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    long dx = abs(x2 - x1);
    long dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    long err = dx + dy; 
    long e2;
    bool need_to_draw = true;

    while (true)
    {
        if (need_to_draw)
        {
            if (x1 > x_right[y1])
            {
                x_left[y1] = x_right[y1];
                x_right[y1] = x1;
            }
            else if (x_left[y1] < 0 || x1 < x_left[y1])
            {
                x_left[y1] = x1;
            }
            need_to_draw = false;
        }

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
            need_to_draw = true;
        }
    }
}
