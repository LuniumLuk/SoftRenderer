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
    DIGIT_A,
    DIGIT_B,
    DIGIT_C,
    DIGIT_D,
    DIGIT_E,
    DIGIT_F,
    DIGIT_G,
    DIGIT_H,
    DIGIT_I,
    DIGIT_J,
    DIGIT_K,
    DIGIT_L,
    DIGIT_M,
    DIGIT_N,
    DIGIT_O,
    DIGIT_P,
    DIGIT_Q,
    DIGIT_R,
    DIGIT_S,
    DIGIT_T,
    DIGIT_U,
    DIGIT_V,
    DIGIT_W,
    DIGIT_X,
    DIGIT_Y,
    DIGIT_Z,
    DIGIT_NUM,
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
#ifdef WIN32
const __INT32_TYPE__ DIGITS_16SEG[] = 
#endif
#ifdef MACOS
const int32_t DIGITS_16SEG[] = 
#endif
{
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
    0b1110001111000100, // A
    0b1110001111000111, // B
    0b1110000001000011, // C
    0b1110001001000111, // D
    0b1110000111000011, // E
    0b1110000111000000, // F
    0b1110000011000111, // G
    0b0010001111000100, // H
    0b1100100000010011, // I
    0b1100100000010010, // J
    0b0010010101001000, // K
    0b0010000001000011, // L
    0b0011011001000100, // M
    0b0011001001001100, // N
    0b1110001001000111, // O
    0b1110001111000000, // P
    0b1110001001001111, // Q
    0b1110001111001000, // R
    0b1110000110000111, // S
    0b1100100000010000, // T
    0b0010001001000111, // U
    0b0010001001000111, // V
    0b0010001001101100, // W
    0b0001010000101000, // X
    0b0001010000010000, // Y
    0b1100010000100011, // Z
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
const float DIGITS_16SEG_COORDS[][2] = 
{
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

const int DIGITS_16SEG_EDGES[][2] = 
{
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
    assert(character < DIGIT_NUM);
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

/**
 * draw integer
 * (sign) <-size * gap(pixel)-> digit0 <-size * gap(pixel)-> digit1
 */
void drawString(
    const FrameBuffer & frame_buffer, 
    const float & x, const float & y, 
    const char * string,
    const float & size,
    const RGBCOLOR & color,
    const float & gap = 0.5f,
    const float & ratio = 1.5f, 
    const int & segment = 16 )
{
    size_t length = strlen(string);
    float offset_x = x;
    for (size_t i = 0; i < length; i++)
    {
        assert(string[i] >= 'a' && string[i] <= 'z' || string[i] >= 'A' && string[i] <= 'Z' || string[i] == ' ');
        if (string[i] >= 'a' && string[i] <= 'z')
        {
            drawDigit(
                frame_buffer,
                offset_x, y, (DIGIT_CHARACTER)(DIGIT_A + string[i] - 'a'),
                size, color, ratio, segment
            );
        }
        else if (string[i] >= 'A' && string[i] <= 'Z')
        {
            drawDigit(
                frame_buffer,
                offset_x, y, (DIGIT_CHARACTER)(DIGIT_A + string[i] - 'A'),
                size, color, ratio, segment
            );
        }
        offset_x += size * (1.0f + gap);
    }
}

}

#endif