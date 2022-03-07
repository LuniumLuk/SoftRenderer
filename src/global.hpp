#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

namespace Lurdr
{

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define clamp(a,b,c) min(max((a),(b)),(c))

#define __unused_variable(var) (void(var))

#define PI 3.14159265358979323846264338327950288f
#define EPSILON 1e-6f

typedef unsigned char       byte_t;  // 1 bytes
typedef unsigned short      UINT16;  // 2 bytes
typedef short               INT16;   // 2 bytes
typedef unsigned int        UINT32;  // 4 bytes
typedef int                 INT32;   // 4 bytes
typedef unsigned long long  UINT64;  // 8 bytes
typedef long long           INT64;   // 8 bytes

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
    RGBCOLOR(float r, float g, float b)
    {
        R = clamp(r, 0, 255);
        G = clamp(g, 0, 255);
        B = clamp(b, 0, 255);
    }
} RGBColor;

const RGBColor COLOR_WHITE(255.0f, 255.0f, 255.0f);
const RGBColor COLOR_BLACK(0.0f, 0.0f, 0.0f);

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