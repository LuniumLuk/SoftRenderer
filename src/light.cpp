#include "light.hpp"

using namespace LuGL;

#define _BLINN_PHONG_

LightComp DirectionalLight::getLight(vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    __unused_variable(frag_pos);

    normal.normalize();

    float lambertian = max((-m_direction).dot(normal), 0.0f);
#ifdef _BLINN_PHONG_
    vec3 halfway_dir = (-m_direction + view_dir).normalized();
    float spec = powf(max(normal.dot(halfway_dir), 0.0f), 32.0f);
#else
    vec3 reflect_dir = vec3::reflect(m_direction, normal).normalized();
    float spec = powf(max(view_dir.dot(reflect_dir), 0.0f), 32.0f);
#endif

    LightComp comp = {
        .diffuse = m_diffuse * lambertian,
        .specular = m_specular * spec
    };
    return comp;
}

LightComp PointLight::getLight(vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    normal.normalize();

    vec3 light_dir = m_position - frag_pos;
    float distance = light_dir.length();
    light_dir.normalize();
    float lambertian = max(light_dir.dot(normal), 0.0f);
#ifdef _BLINN_PHONG_
    vec3 halfway_dir = (light_dir + view_dir).normalized();
    float spec = powf(max(normal.dot(halfway_dir), 0.0f), 32.0f);
#else
    vec3 reflect_dir = vec3::reflect(-light_dir, normal);
    float spec = powf(max(view_dir.dot(reflect_dir), 0.0f), 32.0f);
#endif
    // attenuation
    float attenuation = 1.0f / (m_constant + m_linear * distance + m_quadratic * distance * distance);

    LightComp comp = {
        .diffuse = m_diffuse * lambertian * attenuation,
        .specular = m_specular * spec * attenuation
    };
    return comp;
}


