#include <stdio.h>
#include <stdlib.h>
#include "../api.hpp"

using namespace Lurdr;

int main()
{
    vec3 v1(1.0f, 0.0f, 2.5f);
    v1.print();
    vec3 v2(2.5f, -1.4f, 0.3f);
    v2.print();

    vec3 n = v1.cross(v2).normalized();
    n.print();

    printf("%f %f\n", v1.dot(n), v2.dot(n));

    return 0;
}