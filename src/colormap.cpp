#include "colormap.hpp"

using namespace Lurdr;

RGBCOLOR Lurdr::getColorFromScheme(float percentage, const Vector3 * colormap, int map_size)
{
    Vector3 color;
    float s = 1.0f / (map_size - 1);
    int i = (int)(percentage * (map_size - 1));
    float lerp_factor = (percentage - (float)i * s) * (map_size - 1);

    color = Vector3::lerp(colormap[i], colormap[i + 1], lerp_factor);

    return RGBCOLOR(color.x, color.y, color.z);
}

RGBCOLOR Lurdr::getColorMap(float value, float lower, float upper, COLORMAP_TYPE type)
{
    float range = upper - lower;
    assert(fabs(range) > EPSILON);

    switch (type)
    {
        case COLORMAP_PARULA:
            return getColorFromScheme((value - lower) / range, parula_colormap, COLORMAP_PARULA_SIZE);
        case COLORMAP_ACCENT:
            return getColorFromScheme((value - lower) / range, accent_colormap, COLORMAP_ACCENT_SIZE);
        case COLORMAP_BLUES:
            return getColorFromScheme((value - lower) / range, blues_colormap, COLORMAP_BLUES_SIZE);
        case COLORMAP_BRBG:
            return getColorFromScheme((value - lower) / range, brbg_colormap, COLORMAP_BRBG_SIZE);
        case COLORMAP_BUGN:
            return getColorFromScheme((value - lower) / range, bugn_colormap, COLORMAP_BUGN_SIZE);
        case COLORMAP_BUPU:
            return getColorFromScheme((value - lower) / range, bupu_colormap, COLORMAP_BUPU_SIZE);
        case COLORMAP_CHROMAJS:
            return getColorFromScheme((value - lower) / range, chromajs_colormap, COLORMAP_CHROMAJS_SIZE);
        case COLORMAP_DARK2:
            return getColorFromScheme((value - lower) / range, dark2_colormap, COLORMAP_DARK2_SIZE);
        case COLORMAP_GNBU:
            return getColorFromScheme((value - lower) / range, gnbu_colormap, COLORMAP_GNBU_SIZE);
        case COLORMAP_WHGNBU:
            return getColorFromScheme((value - lower) / range, whgnbu_colormap, COLORMAP_WHGNBU_SIZE);
        case COLORMAP_GNPU:
            return getColorFromScheme((value - lower) / range, gnpu_colormap, COLORMAP_GNPU_SIZE);
        case COLORMAP_GREENS:
            return getColorFromScheme((value - lower) / range, greens_colormap, COLORMAP_GREENS_SIZE);
        case COLORMAP_GREYS:
            return getColorFromScheme((value - lower) / range, greys_colormap, COLORMAP_GREYS_SIZE);
        case COLORMAP_ORANGES:
            return getColorFromScheme((value - lower) / range, oranges_colormap, COLORMAP_ORANGES_SIZE);
        case COLORMAP_ORRD:
            return getColorFromScheme((value - lower) / range, orrd_colormap, COLORMAP_ORRD_SIZE);
        case COLORMAP_PAIRED:
            return getColorFromScheme((value - lower) / range, paired_colormap, COLORMAP_PAIRED_SIZE);
        case COLORMAP_PASTEL1:
            return getColorFromScheme((value - lower) / range, pastel1_colormap, COLORMAP_PASTEL1_SIZE);
        case COLORMAP_PASTEL2:
            return getColorFromScheme((value - lower) / range, pastel2_colormap, COLORMAP_PASTEL2_SIZE);
        case COLORMAP_PIYG:
            return getColorFromScheme((value - lower) / range, piyg_colormap, COLORMAP_PIYG_SIZE);
        case COLORMAP_PRGN:
            return getColorFromScheme((value - lower) / range, prgn_colormap, COLORMAP_PRGN_SIZE);
        case COLORMAP_PUBUGN:
            return getColorFromScheme((value - lower) / range, pubugn_colormap, COLORMAP_PUBUGN_SIZE);
        case COLORMAP_PUBU:
            return getColorFromScheme((value - lower) / range, pubu_colormap, COLORMAP_PUBU_SIZE);
        case COLORMAP_PUOR:
            return getColorFromScheme((value - lower) / range, puor_colormap, COLORMAP_PUOR_SIZE);
        case COLORMAP_PURD:
            return getColorFromScheme((value - lower) / range, purd_colormap, COLORMAP_PURD_SIZE);
        case COLORMAP_PURPLES:
            return getColorFromScheme((value - lower) / range, purples_colormap, COLORMAP_PURPLES_SIZE);
        case COLORMAP_RDBU:
            return getColorFromScheme((value - lower) / range, rdbu_colormap, COLORMAP_RDBU_SIZE);
        case COLORMAP_RDGY:
            return getColorFromScheme((value - lower) / range, rdgy_colormap, COLORMAP_RDGY_SIZE);
        case COLORMAP_RDPU:
            return getColorFromScheme((value - lower) / range, rdpu_colormap, COLORMAP_RDPU_SIZE);
        case COLORMAP_RDYLBU:
            return getColorFromScheme((value - lower) / range, rdylbu_colormap, COLORMAP_RDYLBU_SIZE);
        case COLORMAP_RDYLGN:
            return getColorFromScheme((value - lower) / range, rdylgn_colormap, COLORMAP_RDYLGN_SIZE);
        case COLORMAP_REDS:
            return getColorFromScheme((value - lower) / range, reds_colormap, COLORMAP_REDS_SIZE);
        case COLORMAP_SAND:
            return getColorFromScheme((value - lower) / range, sand_colormap, COLORMAP_SAND_SIZE);
        case COLORMAP_SET1:
            return getColorFromScheme((value - lower) / range, set1_colormap, COLORMAP_SET1_SIZE);
        case COLORMAP_SET2:
            return getColorFromScheme((value - lower) / range, set2_colormap, COLORMAP_SET2_SIZE);
        case COLORMAP_SET3:
            return getColorFromScheme((value - lower) / range, set3_colormap, COLORMAP_SET3_SIZE);
        case COLORMAP_SPECTRAL:
            return getColorFromScheme((value - lower) / range, spectral_colormap, COLORMAP_SPECTRAL_SIZE);
        case COLORMAP_WHYLRD:
            return getColorFromScheme((value - lower) / range, whylrd_colormap, COLORMAP_WHYLRD_SIZE);
        case COLORMAP_YLGNBU:
            return getColorFromScheme((value - lower) / range, ylgnbu_colormap, COLORMAP_YLGNBU_SIZE);
        case COLORMAP_YLGN:
            return getColorFromScheme((value - lower) / range, ylgn_colormap, COLORMAP_YLGN_SIZE);
        case COLORMAP_YLORBR:
            return getColorFromScheme((value - lower) / range, ylorbr_colormap, COLORMAP_YLORBR_SIZE);
        case COLORMAP_YLORRD:
            return getColorFromScheme((value - lower) / range, ylorrd_colormap, COLORMAP_YLORRD_SIZE);
        case COLORMAP_YLRD:
            return getColorFromScheme((value - lower) / range, ylrd_colormap, COLORMAP_YLRD_SIZE);
        case COLORMAP_NUM:
            break;
        default:
            break;
    }
    return COLOR_BLACK;
}