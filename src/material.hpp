#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.hpp"
#include "image.hpp"
#include "maths.hpp"

namespace Lurdr
{

class Texture
{
private:
    mutable vec4 m_base_color;
    UniformImage *m_texture_surface;
    bool         m_need_delete;

public:
    Texture():
        m_base_color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_texture_surface(nullptr),
        m_need_delete(false) {}
    Texture(const vec4 & base_color, UniformImage * texture_surface):
        m_base_color(base_color),
        m_texture_surface(texture_surface),
        m_need_delete(false) {}
    Texture(UniformImage * texture_surface):
        m_base_color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_texture_surface(texture_surface),
        m_need_delete(false) {}
    Texture(const char * filename);
    Texture(const vec4 & base_color, const char * filename);
    ~Texture();

    void setBaseColor(const vec4 & base_color) const { m_base_color = base_color; }
    vec4 getBaseColor() const { return m_base_color; }
    void setTextureSurface(UniformImage * texture_surface);
    void loadTextureSurface(const char * filename);
    UniformImage* getTextureSurface() const { return m_texture_surface; }

    vec4 sampleAt(const vec2 & texcoord) const;
    static vec4 sampler(const Texture & texture, const vec2 & texcoord);

    void printSurfaceInfo() const { if (m_texture_surface) m_texture_surface->printImageInfo(); }
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
        normal(Texture()) {}
    
};

}

#endif