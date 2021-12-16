#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "global.hpp"
#include "buffer.hpp"
#include "image.hpp"

namespace Lurdr
{

void drawPoint(const FrameBuffer & frame_buffer, Vector2 position, RGBColor color);

void drawLine(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, RGBColor color);

void drawTriangle(const FrameBuffer & frame_buffer, Vector2 v1, Vector2 v2, Vector2 v3, RGBColor color);

void drawTexture(const FrameBuffer & frame_buffer); // ...

}

#endif