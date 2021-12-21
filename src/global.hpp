#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

namespace Lurdr
{

// x86_64
typedef unsigned char  byte_t;   // 1 bytes
typedef unsigned short ushort_t; // 2 bytes
typedef unsigned int   ulong_t;  // 4 bytes
typedef int            long_t;   // 4 bytes

typedef struct RGBCOLOR
{
    byte_t R;
    byte_t G;
    byte_t B;
    
    RGBCOLOR(byte_t r, byte_t g, byte_t b)
    {
        R = r;
        G = g;
        B = b;
    }
} RGBColor;

// for hdr image
typedef struct RGBECOLOR
{
    byte_t R;
    byte_t G;
    byte_t B;
    byte_t E;

    RGBECOLOR(byte_t r, byte_t g, byte_t b, byte_t e)
    {
        R = r;
        G = g;
        B = b;
        E = e;
    }
} RGBEColor;

}

#endif