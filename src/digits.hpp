#ifndef __DIGITS_HPP__
#define __DIGITS_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.hpp"
#include "maths.hpp"
#include "buffer.hpp"
#include "rasterizer.hpp"

namespace Lurdr
{

// TODO: add alphabet digits -> DIGIT_A
enum DIGIT_CHARACTER
{
    DIGIT_0,
    DIGIT_1,
    DIGIT_2,
    DIGIT_3,
    DIGIT_4,
    DIGIT_5,
    DIGIT_6,
    DIGIT_7,
    DIGIT_8,
    DIGIT_9,
    DEGIT_NUM,
};

/**
 * 16-segment digit map
 *    -0- -1- 
 *   |\  |  /|
 *   2 3 4 5 6
 *   |  \|/  |
 *    -7- -8- 
 *   |  /|\  |
 *   9 A B C D
 *   |/  |  \|
 *    -E- -F- 
 */

// 16-segment digit data
const int32_t DIGITS_16SEG[] = {
    0b1110011001100111, // 0
    0b0000001000000100, // 1
    0b1100001111000011, // 2
    0b1100001110000111, // 3
    0b0010001110000100, // 4
    0b1110000110000111, // 5
    0b1110000111000111, // 6
    0b1100001000000100, // 7
    0b1110001111000111, // 8
    0b1110001110000111, // 9
};

/**
 * 16-segment digit vertices coords
 *   u ---▶
 * v 0 -- 1 -- 2
 * | | \  |  / |
 * ▼ |  \ | /  |
 *   3 -- 4 -- 5
 *   |  / | \  |
 *   | /  |  \ |
 *   6 -- 7 -- 8
 */
const float DIGITS_16SEG_COORDS[][2] = {
    { 0.0f, 0.0f },
    { 0.5f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.5f },
    { 0.5f, 0.5f },
    { 1.0f, 0.5f },
    { 0.0f, 1.0f },
    { 0.5f, 1.0f },
    { 1.0f, 1.0f },
};

const int DIGITS_16SEG_EDGES[][2] = {
    { 0, 1 }, { 1, 2 }, { 0, 3 }, { 0, 4 },
    { 1, 4 }, { 2, 4 }, { 2, 5 }, { 3, 4 },
    { 4, 5 }, { 3, 6 }, { 4, 6 }, { 4, 7 },
    { 4, 8 }, { 5, 8 }, { 6, 7 }, { 7, 8 }, 
};

/**
 * draw digits on frame buffer
 * x,y -- width : size(pixel) --▶ u
 * |
 * |
 * height : size * ratio(pixel)
 * |
 * |
 * ▼
 * v
 */
void drawDigit(
    const FrameBuffer & frame_buffer, 
    const float & x, const float & y, 
    const DIGIT_CHARACTER & character, 
    const float & size, 
    const RGBCOLOR & color,
    const float & ratio = 1.5f, 
    const int & segment = 16 )
{
    assert(segment == 16); // support 16-segment so far

    for (int i = 0; i < 16; i++)
    {
        if (1 & (DIGITS_16SEG[character] >> (15 - i))) {
            drawLine(frame_buffer, Vector2(
                x + DIGITS_16SEG_COORDS[DIGITS_16SEG_EDGES[i][0]][0] * size,
                frame_buffer.getHeight() - y - DIGITS_16SEG_COORDS[DIGITS_16SEG_EDGES[i][0]][1] * size * ratio
            ), Vector2(
                x + DIGITS_16SEG_COORDS[DIGITS_16SEG_EDGES[i][1]][0] * size,
                frame_buffer.getHeight() - y - DIGITS_16SEG_COORDS[DIGITS_16SEG_EDGES[i][1]][1] * size * ratio
            ), color);
        }
    }
}

/**
 * draw integer
 * (sign) <-size * gap(pixel)-> digit0 <-size * gap(pixel)-> digit1
 */
void drawInteger(
    const FrameBuffer & frame_buffer, 
    const float & x, const float & y, 
    int number, 
    const float & size, 
    const RGBCOLOR & color,
    const float & gap = 0.5f,
    const float & ratio = 1.5f, 
    const int & segment = 16 )
{
    bool is_negative = false;
    if (number < 0)
    {
        is_negative = true;
        number = -number;
    }
    DynamicArray<DIGIT_CHARACTER> digits;

    while (number > 0)
    {
        digits.push_back((DIGIT_CHARACTER)(number % 10));
        number /= 10;
    }

    float offset_x = x;
    if (is_negative)
    {
        drawLine(
            frame_buffer,
            Vector2(offset_x, frame_buffer.getHeight() - y - size * ratio * 0.5f),  
            Vector2(offset_x + size, frame_buffer.getHeight() - y - size * ratio * 0.5f),
            color
        );
        offset_x += size * (1.0f + gap);
    }
    for (size_t i = 0; i < digits.size(); i++)
    {
        drawDigit(
            frame_buffer,
            offset_x, y, digits[digits.size() - 1 - i],
            size, color, ratio, segment
        );
        offset_x += size * (1.0f + gap);
    }
}

}

#endif