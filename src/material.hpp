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
    vec4         m_base_color;
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

    void setBaseColor(const vec4 & base_color) { m_base_color = base_color; }
    vec4 getBaseColor() const { return m_base_color; }
    void setTextureSurface(UniformImage * texture_surface)
    {
        m_texture_surface = texture_surface;
        m_texture_surface->convertColorSpace(COLOR_RGB);
    }
    UniformImage* getTextureSurface() const { return m_texture_surface; }

    vec4 sampleAt(const vec2 & texcoord) const;
    static vec4 sampler(const Texture & texture, const vec2 & texcoord);

    void printSurfaceInfo() const { if (m_texture_surface) m_texture_surface->printImageInfo(); }
};

#define SHADER_UNIFORM      void** uniform_standard, void** uniform_light, void** unifrom_custom

#define MODEL_MATRIX        (*(mat4*)uniform_standard[0])
#define VIEW_MATRIX         (*(mat4*)uniform_standard[1])
#define PERSPECTIVE_MATRIX  (*(mat4*)uniform_standard[2])
#define MVP_MATRIX          (MODEL_MATRIX * VIEW_MATRIX * PERSPECTIVE_MATRIX)

struct vdata
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

class v2f
{
public:
    vec4 position;
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoord;
};

class Material
{
public:
    virtual v2f vert(vdata v, SHADER_UNIFORM) = 0;
    virtual vec4 frag(v2f out, SHADER_UNIFORM) = 0;

    Material() {}
};

class LambertianMaterial : public Material
{
public:
    Texture *diffuse;
    
    LambertianMaterial(): Material() {}

    v2f vert(vdata v, SHADER_UNIFORM)
    {
        // standard vertex shader pipeline
        v2f out;

        out.position = MVP_MATRIX * vec4(v.position, 1.0f);
        out.frag_pos = vec3(MODEL_MATRIX * vec4(v.position, 1.0f));
        out.normal   = mat3(MODEL_MATRIX.inversed().transposed()) * v.normal;
        out.texcoord = v.texcoord;

        return out;
    }

    vec4 frag(v2f out, SHADER_UNIFORM)
    {
        // [TODO]
        return vec4(0.2f, 0.5f, 1.0f, 1.0f);
    }
};

class PhongMaterial : public Material
{
public:
    vec3    ambient;
    Texture *diffuse;
    Texture *specular;
    Texture *normal;
    Texture *height;
    Texture *emission;
    float   shininess;

    PhongMaterial(): Material() {}
};

}

#endif