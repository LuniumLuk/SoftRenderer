#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

namespace LuGL
{

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define clamp(a,b,c) min(max((a),(b)),(c))

#define __unused_variable(var) (void(var))

#define PI 3.14159265358979323846264338327950288f
#define EPSILON 1e-6f
#define ASSERT_NON_ZERO(x) assert(fabs(x)>EPSILON)
#define ZERO_FLOAT(x) (fabs(x)<EPSILON)

#define TF_LINEAR true
#define TF_NEAREST false

enum SampleOption {
    LUGL_SAMPLE_DEFAULT,
    LUGL_SAMPLE_2xMSAA,
    LUGL_SAMPLE_4xMSAA,
    LUGL_SAMPLE_8xMSAA,
    LUGL_SAMPLE_OPTION_NUM,
};

class Global
{
public:
    bool wireframe_mode = false;
    bool depth_test = true;
    bool backface_culling = true;
    bool texture_filtering_linear = TF_LINEAR;
    unsigned short sample_option = LUGL_SAMPLE_DEFAULT;

    Global() {}
};

#define LUGL_WIREFRAME_MODE(val)     (Singleton<Global>::get().wireframe_mode=val)
#define LUGL_DEPTH_TEST(val)         (Singleton<Global>::get().depth_test=val)
#define LUGL_BACKFACE_CULLING(val)   (Singleton<Global>::get().backface_culling=val)
#define LUGL_TEXTURE_FILTERING(val)  (Singleton<Global>::get().texture_filtering_linear=val)
#define LUGL_SAMPLE_OPTION(val)      (Singleton<Global>::get().sample_option=val)

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
const RGBColor COLOR_RED(255.0f, 0.0f, 0.0f);
const RGBColor COLOR_GREEN(0.0f, 255.0f, 0.0f);
const RGBColor COLOR_BLUE(0.0f, 0.0f, 255.0f);
const RGBColor COLOR_ORANGE(255.0f, 127.0f, 51.0f);

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

template<typename T>
class Singleton
{
private:
    Singleton() = default;

public:
    static T& get()
    {
        static T instance;
        return instance;
    }

    virtual ~Singleton() noexcept = default;
    Singleton(const Singleton &) = delete;
    Singleton& operator= (const Singleton &) = delete;
};

/* MSAA Specs */

/*  2X MSAA pattern
 *    ------------
 *   |    +       |        (-0.1,  0.4) for case0
 *   |         -  |        ( 0.4,  0.1) for case1
 *   |  -         |        (-0.4, -0.1) for case1
 *   |       +    |        ( 0.1, -0.4) for case0
 *    ------------
 */

/*  4X MSAA pattern
 *    ------------
 *   |    *       |        (-0.1,  0.4)
 *   |         *  |        ( 0.4,  0.1)
 *   |  *         |        (-0.4, -0.1)
 *   |       *    |        ( 0.1, -0.4)
 *    ------------
 */

/*  8X MSAA pattern
 *    ------------
 *   | *     *    |        (-0.375,  0.375) (0.125,  0.375)
 *   |    *     * |        (-0.125,  0.125) (0.375,  0.125)
 *   | *     *    |        (-0.375, -0.125) (0.125, -0.125)
 *   |    *     * |        (-0.125, -0.375) (0.375, -0.375)
 *    ------------
 */

const float LUGL_2xMSAA_PATTERN[][2] = {
    {-0.1,  0.4},
    { 0.1, -0.4},
};

const float LUGL_4xMSAA_PATTERN[][2] = {
    {-0.1,  0.4},
    { 0.4,  0.1},
    {-0.4, -0.1},
    { 0.1, -0.4},
};

const float LUGL_8xMSAA_PATTERN[][2] = {
    {-0.375,  0.375}, {0.125,  0.375},
    {-0.125,  0.125}, {0.375,  0.125},
    {-0.375, -0.125}, {0.125, -0.125},
    {-0.125, -0.375}, {0.375, -0.375},
};

}

#endif