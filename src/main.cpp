#include <stdlib.h>
#include <stdio.h>
#include "../include/lmath.h"
#include <time.h>

using namespace Lurdr;

typedef struct { float x, y, z; } vec3s;

void test_vec_alloc() {
    double dur;
    clock_t start, end;
    start = clock();

    int iter_times = 10000;
    vec3* vec3_array = new vec3[iter_times];
    for (int i = 1; i < iter_times; i++) {
        vec3_array[i].x = 1;
        vec3_array[i].y = 1;
        vec3_array[i].z = 1;
    }

    end = clock();
    dur = (double)(end - start);
    printf("Vector3 Class Use Time:%f\n", (dur / CLOCKS_PER_SEC));
    start = clock();

    vec3s* vec3s_array = new vec3s[iter_times];
    for (int i = 0; i < iter_times; i++) {
        vec3s_array[i].x = 1;
        vec3s_array[i].y = 2;
        vec3s_array[i].z = 3;
    }

    end = clock();
    dur = (double)(end - start);
    printf("Vector3 Struct Use Time:%f\n", (dur / CLOCKS_PER_SEC));
}

void test_quat() {
    int iter_times = 100;
    quat q = quat(1, 2, 3, 4);
    for (int i = 1; i < iter_times; i++) {
        // quats[i] = (quats[i-1] * quats[i-2]).normalized();
        q = q * quat(2, 1, 1, 1);
        q = q.normalized();
        q.print();
    }
}

int main() {
    vec3 from(0.0, 0.0, 1.0);
    vec3 to(0.0, 0.5, 0.866025);


    quat a1 = quat::fromToRotation(from, to).normalized();
    a1.print();


    vec3 vec(0.0, 0.0, 1.0);
    quat a2 = quat::fromAxisAngle(vec3(1.0, 0.0, 0.0), -PI / 6);
    a2.print();

    vec.rotated(a1).print();
    vec.rotated(a2).print();

    return 0;
}