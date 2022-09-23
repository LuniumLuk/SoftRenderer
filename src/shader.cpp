#include "shader.hpp"

using namespace LuGL;

v2f UnlitShader::vert(const vdata in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    v2f out;

    out.position = MVP_MATRIX * vec4(in.position, 1.0f);
    out.texcoord = in.texcoord;
    out.normal   = MODEL_INV_TRANSPOSE * in.normal;

    return out;
}

v2f LitShader::vert(const vdata in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    v2f out;

    out.position  = MVP_MATRIX * vec4(in.position, 1.0f);
    out.frag_pos  = vec3(MODEL_MATRIX * vec4(in.position, 1.0f));
    out.texcoord  = in.texcoord;
    out.normal    = MODEL_INV_TRANSPOSE * in.normal;
    out.tangent   = MODEL_INV_TRANSPOSE * in.tangent;
    out.bitangent = MODEL_INV_TRANSPOSE * in.bitangent;

    return out;
}

vec4 UnlitShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(scene);
    
    return SAMPLER_2D(TEXTURE_ALBEDO, in.texcoord);
}

vec4 TriangleNormalShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    vec3 normal = in.t_normal.normalized();
    rgb color = rgb(
        normal.x * 0.5f + 0.5f,
        normal.y * 0.5f + 0.5f,
        normal.z * 0.5f + 0.5f
    );
    
    return vec4(color, 1.0f);
}

vec4 VertexNormalShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    vec3 normal = in.normal.normalized();
    rgb color = rgb(
        normal.x * 0.5f + 0.5f,
        normal.y * 0.5f + 0.5f,
        normal.z * 0.5f + 0.5f
    );
    
    return vec4(color, 1.0f);
}

vec4 DepthShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    __unused_variable(entity);
    __unused_variable(scene);

    RGBColor rgb_color = getColorMap(in.position.z, 0.0f, 1.0f, COLORMAP_ACCENT);
    rgb color = rgb(
        (float)rgb_color.R / 255.0f,
        (float)rgb_color.G / 255.0f,
        (float)rgb_color.B / 255.0f
    );
    
    return vec4(color, 1.0f);
}

vec4 BlinnPhongShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    const float ambient_strength = 0.1f;
    const float diffuse_strength = 1.0f;

    vec3 normal = vec3(SAMPLER_2D(TEXTURE_NORMAL, in.texcoord)) * 2.0f - vec3(1.0f, 1.0f, 1.0f);
    float specular_strength = (SAMPLER_2D(TEXTURE_SPECULAR, in.texcoord)).length();
    normal = (TBN_MATRIX * normal).normalized();

    vec3 view_dir = (scene.getCamera().getPosition() - in.frag_pos).normalized();
    LightComp light_comp = scene.calcLight(normal, in.frag_pos, view_dir);
    vec3 color = ambient_strength * vec3(SAMPLER_2D(TEXTURE_ALBEDO, in.texcoord)) +
                 diffuse_strength * light_comp.diffuse.multiply(vec3(SAMPLER_2D(TEXTURE_DIFFUSE, in.texcoord))) +
                 specular_strength * light_comp.specular.multiply(vec3(SAMPLER_2D(TEXTURE_SPECULAR, in.texcoord)));

    return vec4(color, 1.0f);
}

vec4 NormalMappingShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    vec3 normal = vec3(SAMPLER_2D(TEXTURE_NORMAL, in.texcoord)) * 2.0f - vec3(1.0f, 1.0f, 1.0f);
    normal = (TBN_MATRIX * normal).normalized();

    vec3 normal_color = (normal + vec3(1.0f, 1.0f, 1.0f)) * 0.5f;
    return vec4(normal_color, 1.0f);
}