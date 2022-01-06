#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "maths.hpp"
#include "image.hpp"
#include "buffer.hpp"
#include "scene.hpp"

namespace Lurdr
{

#define MAX_UNIFORM_COUNT 64

#define SHADER_FUNC(name) void(*name)(int,void**,void**,int,void**)
#define SHADER_PARAM    int in_count, void* in[], void* uniform[], int out_count, void* out[]
#define layout_in(T,b)  static_cast<T*>(in[b]);assert(b>=0&&b<in_count)
#define uniform(T,b)    static_cast<T*>(uniform[b]);assert(b>=0&&b<MAX_UNIFORM_COUNT)
#define layout_out(T,b) static_cast<T*>(out[b]);assert(b>=0&&b<out_count)


enum SHADER_TYPE
{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER
};

class Program
{
private:
    SHADER_FUNC(m_vertex_shader);
    SHADER_FUNC(m_fragment_shader);
    SHADER_FUNC(m_geometry_shader);

    void*   m_uniform[MAX_UNIFORM_COUNT];

public:
    Program();
    ~Program() {}

    void linkShader(SHADER_TYPE type, SHADER_FUNC(shader));
    void run(SHADER_TYPE type, int in_count, void* in[], int out_count, void* out[]);
    void setUnifom(int idx, void* uniform);
};

struct PointLight
{
    Vector3 position;
    float   constant;
    float   linear;
    float   quadratic;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
};

struct PointLightSimple
{
    Vector3 position;
    float   constant;
    float   linear;
    float   quadratic;
    Vector3 color;
};

void uniformShader(
    int in_count,
    void* in[],
    int uniform_count,
    void* uniform[],
    int out_count,
    void* out[] 
);

void simpleVertexShader(SHADER_PARAM);
void simpleFragmentShader(SHADER_PARAM);

void vertexShaderSimple(
    const Vector3 & in_position,
    const Matrix4 & uniform_model,
    const Matrix4 & uniform_view,
    const Matrix4 & uniform_projection,
    Vector4 & out_position,
    Vector3 & out_fragment_position
);

// support one point light and one uniform diffuse color
void fragmentShaderSimple(
    const Vector3 & in_fragment_position,
    const Vector2 & in_triangle_normal,
    const Vector3 & uniform_view_position,
    const Vector3 & uniform_color,
    const PointLight & uniform_point_light,
    Vector4 & out_fragment_color
);

void vertexShaderPhong(
    const Vector3 & in_position,
    const Vector3 & in_normal,
    const Vector2 & in_texcoord,
    const Matrix4 & uniform_model,
    const Matrix4 & uniform_view,
    const Matrix4 & uniform_projection,
    Vector4 & out_position,
    Vector3 & out_fragment_position,
    Vector2 & out_texcoord
);
void fragmentShaderPhong(
    const Vector3 & in_fragment_position,
    const Vector2 & in_texcoord,
    const Vector3 & uniform_view_position,
    const UniformImage & uniform_texture_diffuse,
    const UniformImage & uniform_texture_specular,
    const UniformImage & uniform_texture_normal,
    const PointLight & uniform_point_light,
    Vector4 & out_fragment_color
);

}

#endif