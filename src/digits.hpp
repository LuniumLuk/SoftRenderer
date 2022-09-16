#ifndef __DIGITS_HPP__
#define __DIGITS_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.hpp"
#include "maths.hpp"

namespace LuGL
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
    DIGIT_DASH,
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
const INT32 DIGITS_16SEG[] = 
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
    0b0000000110000000, // - (DASH)
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

const long DIGITS_16SEG_EDGES[][2] = 
{
    { 0, 1 }, { 1, 2 }, { 0, 3 }, { 0, 4 },
    { 1, 4 }, { 2, 4 }, { 2, 5 }, { 3, 4 },
    { 4, 5 }, { 3, 6 }, { 4, 6 }, { 4, 7 },
    { 4, 8 }, { 5, 8 }, { 6, 7 }, { 7, 8 }, 
};

}

#endif