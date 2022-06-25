#include "envmap.hpp"

using namespace Lurdr;

// #define V2THETA(v) (2.0f * acosf(sqrtf(1 - (v))))
#define V2THETA(v) (PI * (v))
#define U2PHI(u) (2.0f * PI * (u))

Envmap::Envmap(const char * filename)
{
    size_t len = strlen(filename);
    if (strncmp(&filename[len - 4], "bmp", 3) == 0 || strncmp(&filename[len - 4], "BMP", 3) == 0)
    {
        printf("support only BMP format\n");
        return;
    }

    BMPImage bmp_image(filename);
    m_envmap_surface = new UniformImage(bmp_image);
    m_envmap_surface->convertColorSpace(COLOR_RGB);

    calcSHCoefficients();
}

Envmap::~Envmap()
{
    delete m_envmap_surface;
}

// convert spherical coordinates to cartesian coordinates
static vec3 sh2car(const vec2 & sh)
{
    return vec3(
        sinf(sh.theta) * cos(sh.phi),
        sinf(sh.theta) * sin(sh.phi),
        cosf(sh.theta)
    );
}

// convert cartesian coordinates to spherical coordinates
static vec2 car2sh(const vec3 & car)
{
    const float r = car.length();
    return vec2(
        acosf(car.z / r),    // theta
        atanf(car.y / car.x) // phi
    );
}

static float getY(long l, long m, const vec2 & sh)
{
    const vec3 car = sh2car(sh);
    if (l == 0 && m == 0)
    {
        return 0.282095f;
    }
    else if (l == 1)
    {
        switch (m)
        {
            case 1:
                return 0.488603f * car.x;
            case 0:
                return 0.488603f * car.z;
            case -1:
                return 0.488603f * car.y;
            default:
                return 0.0f;
        }
    }
    else if (l == 2)
    {
        switch (m)
        {
            case 2:
                return 0.546274f * (car.x * car.x - car.y * car.y);
            case 1:
                return 1.092548f * car.x * car.z;
            case 0:
                return 0.315392f * (3.0f * car.z * car.z - 1.0f);
            case -1:
                return 1.092548f * car.y * car.z;
            case -2:
                return 1.092548f * car.x * car.y;
            default:
                return 0.0f;
        }
    }
    return 0.0f;
}

static float getAHat(long l)
{
    switch (l)
    {
        case 0:
            return 3.141593f;
        case 1:
            return 2.094395f;
        case 2:
            return 0.785398f;
        default:
            return 0.0f;
    }
}

static long getSHIndex(long l, long m)
{
    switch (l)
    {
        case 0:
            return 0;
        case 1:
            return 2 + m;
        case 2:
            return 6 + m;
        default:
            return 0;
    }
}

float Envmap::calcE(const vec2 & sh, long channel) const
{
    const vec3 car = sh2car(sh);
    return (
        0.429043f * m_L[getSHIndex(2, 2) * 3 + channel] * (car.x * car.x - car.y * car.y) +
        0.743125f * m_L[getSHIndex(2, 0) * 3 + channel] * car.z * car.z +
        0.886227f * m_L[getSHIndex(0, 0) * 3 + channel] -
        0.247708f * m_L[getSHIndex(2, 0) * 3 + channel] +
        2.0f * 0.429043f * (
            m_L[getSHIndex(2, -2) * 3 + channel] * car.x * car.y +
            m_L[getSHIndex(2,  1) * 3 + channel] * car.x * car.z +
            m_L[getSHIndex(2, -1) * 3 + channel] * car.y * car.z
        ) +
        2.0f * 0.511664f * (
            m_L[getSHIndex(1,  1) * 3 + channel] * car.x +
            m_L[getSHIndex(1, -1) * 3 + channel] * car.y +
            m_L[getSHIndex(1,  0) * 3 + channel] * car.z
        )
    );
}

rgb Envmap::calcIrradiance(const vec3 & view_dir) const
{
    const vec2 sh = car2sh(view_dir);
    rgb irradiance(0.0f, 0.0f, 0.0f);
    for (long l = 0; l < 3; l++)
    {
        for (long m = -l; m < l + 1; m++)
        {
            const float Y_lm = getY(l, m, sh);
            const float A_hat = getAHat(l);
            irradiance.r += A_hat * m_L[getSHIndex(l, m) * 3] * Y_lm;
            irradiance.g += A_hat * m_L[getSHIndex(l, m) * 3 + 1] * Y_lm;
            irradiance.b += A_hat * m_L[getSHIndex(l, m) * 3 + 2] * Y_lm;
        }
    }
    return irradiance;
}

rgb Envmap::calcIrradiance(const vec2 & sh) const
{
    rgb irradiance(0.0f, 0.0f, 0.0f);
    for (long l = 0; l < 3; l++)
    {
        for (long m = -l; m < l + 1; m++)
        {
            const float Y_lm = getY(l, m, sh);
            const float A_hat = getAHat(l);
            irradiance.r += A_hat * m_L[getSHIndex(l, m) * 3] * Y_lm;
            irradiance.g += A_hat * m_L[getSHIndex(l, m) * 3 + 1] * Y_lm;
            irradiance.b += A_hat * m_L[getSHIndex(l, m) * 3 + 2] * Y_lm;
        }
    }
    return irradiance;
}

rgb Envmap::calcIrradianceFast(const vec3 & view_dir) const
{
    const vec2 sh = car2sh(view_dir);
    return rgb(
        calcE(sh, 0),
        calcE(sh, 1),
        calcE(sh, 2)
    );
}

rgb Envmap::calcIrradianceFast(const vec2 & sh) const
{
    return rgb(
        calcE(sh, 0),
        calcE(sh, 1),
        calcE(sh, 2)
    );
}

vec2 Envmap::getSH(long x, long y)
{
    float U = (float)x / (m_envmap_surface->getImageWidth() - 1);
    float V = (float)y / (m_envmap_surface->getImageHeight() - 1);
    return vec2(
        V2THETA(V), // theta
        U2PHI(U)    // phi
    );
}

void Envmap::calcSHCoefficients()
{
    float surface_integral = 0.0f;
    long im_w = m_envmap_surface->getImageWidth();
    long im_h = m_envmap_surface->getImageHeight();
    for (long y = 0; y < im_h; y++)
    {
        float V = (float)y / (im_h - 1);
        float theta = V2THETA(V);
        surface_integral += sinf(theta) * im_w;
    }
    float factor = PI / surface_integral;

    for (long l = 0; l < 3; l++)
    {
        for (long m = -l; m < l + 1; m++)
        {
            m_L[getSHIndex(l, m) * 3] = 0.0f;
            m_L[getSHIndex(l, m) * 3 + 1] = 0.0f;
            m_L[getSHIndex(l, m) * 3 + 2] = 0.0f;
            for (long x = 0; x < im_w; x++)
            {
                for (long y = 0; y < im_h; y++)
                {
                    const vec2 sh = getSH(x, y);
                    const float Y_lm = getY(l, m, sh);
                    m_L[getSHIndex(l, m) * 3] += (float)(*m_envmap_surface)(y, x, 0) * Y_lm * sinf(sh.theta);
                    m_L[getSHIndex(l, m) * 3 + 1] += (float)(*m_envmap_surface)(y, x, 1) * Y_lm * sinf(sh.theta);
                    m_L[getSHIndex(l, m) * 3 + 2] += (float)(*m_envmap_surface)(y, x, 2) * Y_lm * sinf(sh.theta);
                }
            }
            m_L[getSHIndex(l, m) * 3] *= factor / 255.0f;
            m_L[getSHIndex(l, m) * 3 + 1] *= factor / 255.0f;
            m_L[getSHIndex(l, m) * 3 + 2] *= factor / 255.0f;
        }
    }
}

rgb Envmap::getPixel(float theta, float phi) const
{
    if (theta < 0.0f)
    {
        theta += PI;
        phi -= PI;
    }
    if (phi < 0.0f)
    {
        phi += 2.0f * PI;
    }

    assert(theta >= 0.0f && theta <= PI);
    assert(phi >= 0.0f && phi <= PI * 2.0f);

    rgb color = UniformImage::sampler(*m_envmap_surface, vec2(
        phi / (2.0f * PI),
        theta / PI
    ));

    return color / 255.0f;
}

LightComp EnvLight::getLight(vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    __unused_variable(frag_pos);
    __unused_variable(view_dir);

    const float phi = 0.0f;
    const float theta = 0.0f;
    rgb color = m_envmap.getPixel(phi, theta);

    LightComp comp = {
        .diffuse = m_diffuse * lambertian,
        .specular = m_specular * spec
    };
    return comp;
}