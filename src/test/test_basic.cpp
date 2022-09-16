#include "test.hpp"

using namespace LuGL;

#define V2THETA(v) (2.0f * acosf(sqrtf(1 - (v))))
#define U2PHI(u) (2.0f * PI * (u))

int test_basic()
{
#if 0
    BMPImage bmp_image("assets/envmaps/env02.bmp");
    {
        Timer t("read bmp");
        vec3 mean_color = vec3::ZERO;
        for (long x = 0; x < bmp_image.getImageWidth(); x++)
        {
            for (long y = 0; y < bmp_image.getImageHeight(); y++)
            {
                mean_color.b += bmp_image(y, x, 0);
                mean_color.g += bmp_image(y, x, 1);
                mean_color.r += bmp_image(y, x, 2);
            }
        }
        mean_color = mean_color * (1.0f / (bmp_image.getImageWidth() * bmp_image.getImageHeight() * 255));
        mean_color.print();
    }

    Envmap* envmap;
    {
        Timer t("envmap");
        envmap = new Envmap("assets/envmaps/env02.bmp");
    }

    const float *L = envmap->getCoefficients();
    for (int i = 0; i < 9; i++)
    {
        printf("%f %f %f\n", L[i * 3], L[i * 3 + 1], L[i * 3 + 2]);
    }

    {
        Timer t("write bmp");
        for (long x = 0; x < bmp_image.getImageWidth(); x++)
        {
            for (long y = 0; y < bmp_image.getImageHeight(); y++)
            {
                float U = (float)x / (bmp_image.getImageWidth() - 1);
                float V = (float)y / (bmp_image.getImageHeight() - 1);
                const vec2 sh(V2THETA(1 - V), U2PHI(U));
                // vec3 irradiance = envmap->calcIrradianceFast(sh);
                vec3 irradiance = envmap->calcIrradiance(sh);
                bmp_image(y, x, 2) = irradiance.r * 255;
                bmp_image(y, x, 1) = irradiance.g * 255;
                bmp_image(y, x, 0) = irradiance.b * 255;
            }
        }
    }

    {
        Timer t("read bmp");
        vec3 mean_color = vec3::ZERO;
        for (long x = 0; x < bmp_image.getImageWidth(); x++)
        {
            for (long y = 0; y < bmp_image.getImageHeight(); y++)
            {
                mean_color.b += bmp_image(y, x, 0);
                mean_color.g += bmp_image(y, x, 1);
                mean_color.r += bmp_image(y, x, 2);
            }
        }
        mean_color = mean_color * (1.0f / (bmp_image.getImageWidth() * bmp_image.getImageHeight() * 255));
        mean_color.print();
    }

    bmp_image.writeImage("test.bmp");
#endif

    // Texture tex("assets/textures/lenna.bmp");

    // Texture::sampler(tex, vec2(0.2f, 0.31f)).print();
    // Texture::sampler(tex, vec2(0.1f, 0.115f)).print();

    vec4 p = vec4(0, 0, 1, 1);

    Camera camera;

    camera.setFOV(PI / 4);
    camera.setNear(0.1);
    camera.setFar(5000);

    vec4 pp = camera.getProjectMatrix() * p;

    pp.w = 1.0f / pp.w;
    pp.x *= pp.w;
    pp.y *= pp.w;
    pp.z *= pp.w;

    pp.print();

    return 0;
}