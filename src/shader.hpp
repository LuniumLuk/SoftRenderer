#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "maths.hpp"
#include "image.hpp"

namespace Lurdr
{

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