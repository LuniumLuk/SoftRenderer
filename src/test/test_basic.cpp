#include "test.hpp"

using namespace Lurdr;

static bool cmp(const long & a, const long & b)
{
    return a < b;
}

int test_basic()
{
    long aa[] = { 1, 12, 59, 4, 8, 42, 9, 31, 2, -3, -2, 1 };
    qsort(aa, 0, 11, cmp);
    for (size_t i = 0; i < 12; i++)
    {
        printf("%ld ", aa[i]);
    }
    printf("\n");

    Texture tex("assets/lenna.bmp");

    tex.printSurfaceInfo();
    vec4 color_2 = Texture::sampler(tex, vec2(0.5f, 0.5f));
    color_2.print();

    return 0;
}