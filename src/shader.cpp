#include "shader.hpp"

using namespace Lurdr;

v2f UnlitShader::vert(const vdata in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    v2f out;

    out.position = MVP_MATRIX * vec4(in.position, 1.0f);
    out.texcoord = in.texcoord;
    out.normal   = in.normal;

    return out;
}

vec4 UnlitShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(scene);
    
    return Texture::sampler(entity->getMaterial()->albedo, in.texcoord);
}

vec4 TriangleNormalShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    rgb color = rgb(
        in.t_normal.x * 0.5f + 0.5f,
        in.t_normal.y * 0.5f + 0.5f,
        in.t_normal.z * 0.5f + 0.5f
    );
    
    return vec4(color, 1.0f);
}

vec4 VertexNormalShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    rgb color = rgb(
        in.normal.x * 0.5f + 0.5f,
        in.normal.y * 0.5f + 0.5f,
        in.normal.z * 0.5f + 0.5f
    );
    
    return vec4(color.normalized(), 1.0f);
}