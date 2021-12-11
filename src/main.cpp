#include <stdlib.h>
#include <stdio.h>
#include "../include/lmath.h"
#include <time.h>

using namespace Lurdr;

typedef struct { float x, y, z; } vec3s;

int main() {
    // vec3 a(1, 0, 0);
    // vec3 b(0, -4, 0);

    // vec3 aa = vec3::UNIT_X;
    // vec3 ab = vec3::ZERO;
    // vec3 ac = vec3::UNIT_Y;

    // int dhh = 0;

    // a = -a;
    // vec3 c = a + b + ab;
    // vec3 d = a.cross(b);
    // vec3 e = a * 8;
    // e = (a - b.cross(-a-b));
    // float f = a.dot(b);
    // e = vec3::lerp(c, d, -2);

    

    double dur;
    clock_t start,end;
    start = clock();

    int iter_times = 10000000;
    // vec3* vecs = new vec3[iter_times];
    // for (int i = 0; i < iter_times; i++) {
    //     vecs[i].x = 1;
    //     vecs[i].y = 2;
    //     vecs[i].z = 3;
    // }


    vec3s* vecs = new vec3s[iter_times];
    for (int i = 0; i < iter_times; i++) {
        vecs[i].x = 1;
        vecs[i].y = 2;
        vecs[i].z = 3;
    }

    end = clock();
    dur = (double)(end - start);
    printf("Use Time:%f\n", (dur / CLOCKS_PER_SEC));

    // printf("%f, %f, %f\n", vec.x, vec.y, vec.z);
    // printf("%lu\n", sizeof(float));
    // printf("%lu\n", sizeof(vec));
    // printf("%f, %f, %f\n", d.x, d.y, d.z);
    // printf("%f, %f, %f\n", e.x, e.y, e.z);
    // printf("%f\n", f);

    return 0;
}