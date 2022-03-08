#include "material.hpp"

using namespace Lurdr;

Texture::Texture(const char * filename): 
    m_base_color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
    m_texture_surface(nullptr),
    m_need_delete(false)
{
    size_t len = strlen(filename);
    if (strncmp(&filename[len - 4], "bmp", 3) == 0 || strncmp(&filename[len - 4], "BMP", 3) == 0)
    {
        printf("support only BMP format\n");
        return;
    }

    BMPImage bmp_image(filename);
    m_texture_surface = new UniformImage(bmp_image);
    m_texture_surface->convertColorSpace(COLOR_RGB);
    m_need_delete = true;
}

// delegated constructor : c++11 feature
Texture::Texture(const vec4 & base_color, const char * filename): 
    Texture(filename)
{
    m_base_color = base_color;
}

Texture::~Texture()
{
    if (m_need_delete)
    {
        delete m_texture_surface;
    }
}


vec4 Texture::sampleAt(const vec2 & texcoord) const
{
    if (m_texture_surface)
    {
        vec3 surface_color = UniformImage::sampler(*m_texture_surface, texcoord) / 255.0f;
        vec4 result_color = m_base_color.multiply(vec4(surface_color, 1.0f)); 

        return result_color;
    }
    else
    {
        return m_base_color;
    }
}

vec4 Texture::sampler(const Texture & texture, const vec2 & texcoord)
{
    return texture.sampleAt(texcoord);
}
