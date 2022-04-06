#ifndef __ENVMAP_HPP__
#define __ENVMAP_HPP__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "global.hpp"
#include "image.hpp"

namespace Lurdr
{

class Envmap
{
private:
    UniformImage* m_envmap_surface;
    float m_L[27];

    float calcE(const vec2 & sh, long channel) const;
    vec2 getSH(long x, long y);
    void calcSHCoefficients();

public:
    Envmap() = delete;
    Envmap(const char * filename);
    ~Envmap();

    const float * getCoefficients() const { return &m_L[0]; }

    rgb calcIrradiance(const vec3 & view_dir) const;
    rgb calcIrradiance(const vec2 & sh) const;
    rgb calcIrradianceFast(const vec3 & view_dir) const;
    rgb calcIrradianceFast(const vec2 & sh) const;

    rgb getPixel(float theta, float phi) const;
};

}

#endif