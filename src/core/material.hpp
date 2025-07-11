#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.hpp"
#include "image.hpp"
#include "maths.hpp"

namespace LuGL
{

class Texture
{
private:
    long         m_width;
    long         m_height;
    mutable vec4 m_base_color;
    float        *m_buffer;

public:
    Texture():
        m_width(0),
        m_height(0),
        m_base_color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_buffer(nullptr) {}
    Texture(const vec4 & base_color, const BMPImage & bmp_image);
    Texture(const BMPImage & bmp_image);
    Texture(const char * filename);
    Texture(const vec4 & base_color, const char * filename);
    ~Texture();

    void setBaseColor(const vec4 & base_color) const { m_base_color = base_color; }
    vec4 getBaseColor() const { return m_base_color; }
    void loadTextureSurface(const BMPImage & bmp_image);

    long getTextureWidth() const { return m_width; }
    long getTextureHeight() const { return m_height; }

    vec4 colorAt(long x, long y) const;
    vec4 sampleAt(const vec2 & texcoord) const;
    static vec4 sampler(const Texture & texture, const vec2 & texcoord);
};

class Material
{
public:
    Texture albedo;
    Texture diffuse;
    Texture specular;
    Texture normal;

    Material():
        albedo(Texture()),
        diffuse(Texture()),
        specular(Texture()),
        normal(Texture()) {

        normal.setBaseColor(vec4(0.5f, 0.5f, 1.0f, 1.0f));
    }

};

}

#endif