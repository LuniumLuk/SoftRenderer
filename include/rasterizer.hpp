#ifndef __RASTERIZER_HPP__
#define __RASTERIZER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "global.hpp"
#include "buffer.hpp"
#include "image.hpp"

namespace Lurdr
{

/**
 * basic buffer access api
 */
void drawPixel(const FrameBuffer & frame_buffer, long x, long y, RGBColor color);
void drawScanLine(const FrameBuffer & frame_buffer, long x1, long x2, long y, RGBColor color);

/**
 * basic rasterization method
 */
void drawPoint(const FrameBuffer & frame_buffer, Vector2 position, RGBColor color);
void drawLine(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, RGBColor color);

/**
 * triangle rasterization methods
 *  - standard method
 *  - barycentric method
 *  - bresenham method
 *  - improved bresenham method
 */
void drawTriangle(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color);
void sortVerticesByY(long & x1, long & y1, long & x2, long & y2, long & x3, long & y3);
void standardTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color);
void barycentricTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color);
void standardFillFlatTriangle(const FrameBuffer & frame_buffer, long y12, long y3, float x1, float x2, long x3, RGBColor color);
void bresenhamTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color);
void bresenhamFillFlatTriangle(const FrameBuffer & frame_buffer, long y12, long y3, long x1, long x2, long x3, RGBColor color);
void improvedBresenhamTriangleRasterization(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color);
void fillTriangleTable(long * x_left, long * x_right, long x1, long y1, long x2, long y2, long y_min);

void drawTexture(const FrameBuffer & frame_buffer); // ...

}

#endif