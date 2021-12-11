#include <stdlib.h>
#include <stdio.h>
#include "../include/lmath.h"

using namespace Lurdr;

int main() {
    vec3 a(1, 0, 0);
    vec3 b(0, -4, 0);

    vec3 aa = vec3::UNIT_X;
    vec3 ab = vec3::ZERO;
    vec3 ac = vec3::UNIT_Y;

    int dhh = 0;

    a = -a;
    vec3 c = a + b + ab;
    vec3 d = a.cross(b);
    vec3 e = a * 8;
    e = (a - b.cross(-a-b));
    float f = a.dot(b);
    e = vec3::lerp(c, d, -2);
    printf("%f, %f, %f\n", c.x, c.y, c.z);
    printf("%f, %f, %f\n", d.x, d.y, d.z);
    printf("%f, %f, %f\n", e.x, e.y, e.z);
    printf("%f\n", f);

    return 0;
}