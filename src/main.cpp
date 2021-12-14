#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utility>
#include "../include/lglobal.h"
#include "../include/lmath.h"
#include "../include/limage.h"

using namespace Lurdr;

typedef struct { float x, y, z; } vec3s;

void test_vec_alloc()
{
    double dur;
    clock_t start, end;
    start = clock();

    int iter_times = 10000;
    vec3* vec3_array = new vec3[iter_times];
    for (int i = 1; i < iter_times; i++)
    {
        vec3_array[i].x = 1;
        vec3_array[i].y = 1;
        vec3_array[i].z = 1;
    }

    end = clock();
    dur = (double)(end - start);
    printf("Vector3 Class Use Time: %fs\n", (dur / CLOCKS_PER_SEC));
    start = clock();

    vec3s* vec3s_array = new vec3s[iter_times];
    for (int i = 0; i < iter_times; i++)
    {
        vec3s_array[i].x = 1;
        vec3s_array[i].y = 2;
        vec3s_array[i].z = 3;
    }

    end = clock();
    dur = (double)(end - start);
    printf("Vector3 Struct Use Time: %fs\n", (dur / CLOCKS_PER_SEC));
}

void test_quat()
{
    int iter_times = 100;
    quat q = quat(1, 2, 3, 4);
    for (int i = 1; i < iter_times; i++)
    {
        // quats[i] = (quats[i-1] * quats[i-2]).normalized();
        q = q * quat(2, 1, 1, 1);
        q = q.normalized();
        q.print();
    }
}

void test_rotate()
{
    vec3 from(0.0, 0.0, 1.0);
    vec3 to(0.0, 0.5, 0.866025);


    quat a1 = quat::fromToRotation(from, to).normalized();
    a1.print();


    vec3 vec(0.0, 0.0, 1.0);
    quat a2 = quat::fromAxisAngle(vec3(1.0, 0.0, 0.0), -PI / 6);
    a2.print();

    vec.rotated(a1).print();
    vec.rotated(a2).print();

    printf("%d\n", a1 == a2);
    printf("%d\n", vec.rotated(a1) == vec.rotated(a2));
}

typedef struct TestStruct
{
    float a;
    float b;
    byte_t c;
} tests;

void buffer_test()
{
    size_t len = 10000;
    byte_t * buffer = new byte_t[len];
    for (int i = 0; i < len; i++)
    {
        buffer[i] = (byte_t)(i % 256);
    }

    byte_t * buffer2 = new byte_t[len];
    memcpy(buffer2, buffer, len * sizeof(byte_t));

    for (int i = 1100; i < 1120; i++)
    {
        printf("=%u=", buffer[i]);
    }
    printf("\n");

    delete[] buffer;
    delete[] buffer2;

    printf("%p\n", buffer);
    printf("%p\n", buffer2);

    printf("%u\n", buffer[257]); // still access the correct data, <Undefined Behavior>
}

void image_test()
{
    double dur;
    clock_t start, end;
    start = clock();

    int iter_times = 10000;
    for (int i = 1; i < iter_times; i++)
    {
        BMPImage image("test/lenna.bmp");
        // image.printImageInfo();
        // image.writeImage("test/test.bmp");
    }

    end = clock();
    dur = (double)(end - start);
    printf("Read %d 512 * 512 Images Use Time: %fs\n", iter_times, (dur / CLOCKS_PER_SEC));
}

int main() {
    double dur;
    clock_t start, end;
    start = clock();

    BMPImage image("assets/lenna.bmp");
    // printf("R: %u, G: %u, B: %u\n", image(0, 511, 0), image(0, 511, 1), image(0, 511, 2));
    // printf("R: %u, G: %u, B: %u\n", image(0, 0, 0), image(0, 0, 1), image(0, 0, 2));
    UniformImage u_image(image);

    u_image.printImageInfo();
    size_t iter_time = 1000; // SIMD: ~315ms
                             // normal: ~915ms
    for (size_t i = 0; i < iter_time; i++)
    {
        if (i % 2 == 0) 
            u_image.convertColorSpace(COLOR_RGB);
        else
            u_image.convertColorSpace(COLOR_BGR);
    }

    // u_image.convertColorSpace(COLOR_RGB);
    // printf("R: %u, G: %u, B: %u\n", u_image(511, 511, 0), u_image(511, 511, 1), u_image(511, 511, 2));
    // printf("R: %u, G: %u, B: %u\n", u_image(511, 0, 0), u_image(511, 0, 1), u_image(511, 0, 2));

    end = clock();
    dur = (double)(end - start);
    printf("Use Time: %fms\n", (dur * 1000 / CLOCKS_PER_SEC));

    return 0;
}
