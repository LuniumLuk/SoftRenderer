#include "material.hpp"

using namespace LuGL;

Texture::Texture(const char * filename): Texture()
{
    BMPImage bmp_image(filename);

    loadTextureSurface(bmp_image);
}

// delegated constructor : c++11 feature
Texture::Texture(const vec4 & base_color, const char * filename): 
    Texture(filename)
{
    m_base_color = base_color;
}

Texture::Texture(const vec4 & base_color, const BMPImage & bmp_image):
    Texture(bmp_image)
{
    m_base_color = base_color;
}

Texture::Texture(const BMPImage & bmp_image): Texture()
{
    loadTextureSurface(bmp_image);
}

Texture::~Texture()
{
    if (m_buffer)
        delete[] m_buffer;
}

void Texture::loadTextureSurface(const BMPImage & bmp_image)
{
    if (m_buffer)
    {
        delete[] m_buffer;
    }

    m_width = bmp_image.getImageWidth();
    m_height = bmp_image.getImageHeight();

    long texture_size = m_width * m_height * 4;

    m_buffer = new float[texture_size];
    const byte_t* image_buffer = bmp_image.getImageBufferConst();

    for (long x = 0; x < m_width; x++)
    {
        for (long y = 0; y < m_height; y++)
        {
            long texture_pos = (y * m_width + x) * 4;
            // long texture_pos = ((m_height - y - 1) * m_width + x) * 4;
            long image_pos = (y * m_width + x) * 3;
            m_buffer[texture_pos] = image_buffer[image_pos + 2] / 255.0f;
            m_buffer[texture_pos + 1] = image_buffer[image_pos + 1] / 255.0f;
            m_buffer[texture_pos + 2] = image_buffer[image_pos] / 255.0f;
            m_buffer[texture_pos + 3] = 1.0f;
        }
    }
}

vec4 Texture::sampleAt(const vec2 & texcoord) const
{
    if (m_buffer)
    {
        vec4 surface_color = sampler(*this, texcoord);
        vec4 result_color = m_base_color.multiply(surface_color); 

        return result_color;
    }
    else
    {
        return m_base_color;
    }
}

vec4 Texture::colorAt(long x, long y) const
{
    if (!m_buffer) return m_base_color;
    assert(x >= 0);
    assert(y >= 0);
    if (x >= m_width)
    {
        x = m_width - 1;
    }
    if (y >= m_height)
    {
        y = m_height - 1;
    }

    return vec4(&m_buffer[(y * m_width + x) * 4]);
}


vec4 Texture::sampler(const Texture & texture, const vec2 & texcoord)
{
    if (!texture.m_buffer) return texture.m_base_color;

    if (Singleton<Global>::get().texture_filtering_linear)
    {
        float xf = (float)texture.m_width * clamp(texcoord.u, 0.0f, 1.0f);
        float yf = (float)texture.m_height * clamp(texcoord.v, 0.0f, 1.0f);
        long x = FTOD(xf);
        long y = FTOD(yf);
        float alpha_x = (xf - x);
        float alpha_y = (yf - y);

        vec4 color0 = texture.colorAt(x,     y    );
        vec4 color1 = texture.colorAt(x + 1, y    );
        vec4 color2 = texture.colorAt(x,     y + 1);
        vec4 color3 = texture.colorAt(x + 1, y + 1);

        // duel linear filtering
        return vec4::lerp(
            vec4::lerp(color0, color1, alpha_x),
            vec4::lerp(color2, color3, alpha_x),
            alpha_y
        );
    }
    else
    {
        float xf = (float)texture.m_width * clamp(texcoord.u, 0.0f, 1.0f);
        float yf = (float)texture.m_height * clamp(texcoord.v, 0.0f, 1.0f);
        long x = FTOD(xf);
        long y = FTOD(yf);

        // nearest filtering
        return texture.colorAt(x, y);
    }
}
