#ifndef __IMATH_H__
#define __IMATH_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define min(a,b) (a)<(b)?(a):(b)
#define max(a,b) (a)>(b)?(a):(b)
#define clamp(a,b,c) min(max((a),(b)),(c))

namespace Lurdr {

/**
 * Vector3
 */
class Vector3 {
public:
    float x, y, z;
    Vector3(): x(0), y(0), z(0) {}
    Vector3(float x, float y, float z): x(x), y(y), z(z) {}
    Vector3(const Vector3 & vec): x(vec.x), y(vec.y), z(vec.z) {}

    /* Static Members */
    static Vector3 UNIT_X;
    static Vector3 UNIT_Y;
    static Vector3 UNIT_Z;
    static Vector3 ZERO;

    /* Operator Overload Functions */
    Vector3 operator+ (const Vector3 & other);
    Vector3 operator- ();
    Vector3 operator- (const Vector3 & other);
    Vector3 operator* (const float & multiplicand);
    Vector3 operator/ (const float divisor);
    float dot(const Vector3 & other);
    Vector3 cross(const Vector3 & other);
    Vector3 length();
    Vector3 normalized();

    /* Static Functions */
    static Vector3 lerp(const Vector3 & from, const Vector3 & to, float alpha);

};
typedef Vector3 vec3;

Vector3 operator* (float multiplier, const vec3 & multiplicand);


/**
 * Vector2
 */
class Vector2 {
public:
    float x, y;
};
typedef Vector2 vec2;

}



#endif