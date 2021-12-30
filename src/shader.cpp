#include "shader.hpp"

using namespace Lurdr;

void Lurdr::vertexShaderPhong(
    const Vector3 & in_position,
    const Vector3 & in_normal,
    const Vector2 & in_texcoord,
    const Matrix4 & uniform_model,
    const Matrix4 & uniform_view,
    const Matrix4 & uniform_projection,
    Vector4 & out_position,
    Vector3 & out_fragment_position,
    Vector2 & out_texcoord )
{
    out_texcoord = in_texcoord;
    out_position = (uniform_projection * uniform_view * uniform_model) * Vector4(in_position, 1.0f);
    out_fragment_position = Vector3(uniform_model * Vector4(in_position, 1.0f));
}

Vector3 calcPointLight(
    const PointLight & light,
    const Vector3 & normal,
    const Vector3 & fragment_position,
    const Vector3 & view_direction,
    const Vector2 & texcoord,
    const UniformImage & texture_diffuse,
    const UniformImage & texture_specular )
{
    Vector3 light_direction = (light.position - fragment_position).normalized();

    float diff = max(normal.dot(light_direction), 0.0f);

    Vector3 reflect_direction = Vector3::reflect(-light_direction, normal);
    float spec = powf(max(view_direction.dot(reflect_direction), 0.0f), 32.0f);

    float distance = (light.position - fragment_position).length();
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    Vector3 ambient = light.ambient.multiply(UniformImage::sampler(texture_diffuse, texcoord));
    Vector3 diffuse = (light.diffuse * diff).multiply(UniformImage::sampler(texture_diffuse, texcoord));
    Vector3 specular = (light.specular * spec).multiply(UniformImage::sampler(texture_specular, texcoord));

    return ambient + diffuse + specular;
}

void Lurdr::fragmentShaderPhong(
    const Vector3 & in_fragment_position,
    const Vector2 & in_texcoord,
    const Vector3 & uniform_view_position,
    const UniformImage & uniform_texture_diffuse,
    const UniformImage & uniform_texture_specular,
    const UniformImage & uniform_texture_normal,
    const PointLight & uniform_point_light,
    Vector4 & out_fragment_color )
{
    Vector3 view_direction = (uniform_view_position - in_fragment_position).normalized();

    Vector3 result = calcPointLight(
        uniform_point_light,
        UniformImage::sampler(uniform_texture_normal, in_texcoord),
        in_fragment_position,
        view_direction,
        in_texcoord,
        uniform_texture_normal,
        uniform_texture_specular
    );

    out_fragment_color = Vector4(result, 1.0f);
}