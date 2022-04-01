#include "test.hpp"

using namespace Lurdr;

int test_math() 
{
    vec3 dir(0.0f, 0.5f, -10.0f);
    vec3 axis_y = dir.cross(vec3::UNIT_Y).normalized();

    printf("length: %f\n", dir.length());

    Quaternion rotate_x = Quaternion::fromAxisAngle(vec3::UNIT_Y, 0.1f);
    dir.rotate(rotate_x);

    Quaternion rotate_y = Quaternion::fromAxisAngle(axis_y, 0.1f);
    dir.rotate(rotate_y);

    dir.print();
    printf("length: %f\n", dir.length());

    return 0;
}