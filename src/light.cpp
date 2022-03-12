#include "light.hpp"

using namespace Lurdr;

LightComp DirectionalLight::getLight(vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    __unused_variable(frag_pos);

    float lambertian = max((-m_direction).dot(normal), 0.0f);
    vec3 reflect_dir = vec3::reflect(m_direction, normal);
    float spec = powf(max(view_dir.dot(reflect_dir), 0.0f), 32.0f);

    LightComp comp = {
        .diffuse = m_diffuse * lambertian,
        .specular = m_specular * spec
    };
    return comp;
}

LightComp PointLight::getLight(vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = m_position - frag_pos;
    float distance = light_dir.length();
    light_dir.normalize();
    float lambertian = max(light_dir.dot(normal), 0.0f);
    vec3 reflect_dir = vec3::reflect(-light_dir, normal);
    float spec = powf(max(view_dir.dot(reflect_dir), 0.0f), 32.0f);
    // attenuation
    float attenuation = 1.0f / (m_constant + m_linear * distance + m_quadratic * distance * distance);

    LightComp comp = {
        .diffuse = m_diffuse * lambertian * attenuation,
        .specular = m_specular * spec * attenuation
    };
    return comp;
}


