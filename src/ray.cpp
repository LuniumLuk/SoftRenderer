#include "ray.hpp"

using namespace Lurdr;

// reference : https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
bool Ray::intersectWithTriangle(
    const Vector3 & v1,
    const Vector3 & v2,
    const Vector3 & v3, 
    float * u,
    float * v ) const
{
    Vector3 v12 = v2 - v1;
    Vector3 v13 = v3 - v1;
    Vector3 v23 = v3 - v2;
    Vector3 v31 = v1 - v3;
    Vector3 N = v12.cross(v13);

    float denom = N.dot(N);

    // 1. determine whether the ray is parellel to the triangle
    float N_dot_direction = N.dot(m_direction);
    if (fabs(N_dot_direction) < EPSILON)
    {
        return false;
    }

    // 2. determin whether the triangle is at opposite direction of the ray
    float d = N.dot(v1);
    float t = (N.dot(m_origin) + d) / N_dot_direction;
    if (t < 0)
    {
        return false;
    }

    // 3. get the intersection point
    Vector3 P = m_origin + m_direction * t;

    // 4. inside-out test for P
    Vector3 inside_out_test;

    Vector3 v1P = P - v1;
    inside_out_test = v12.cross(v1P);
    if (N.dot(inside_out_test) < 0)
    {
        return false;
    }

    Vector3 v2P = P - v2;
    inside_out_test = v23.cross(v2P);
    if ((*u = N.dot(inside_out_test)) < 0)
    {
        return false;
    }

    Vector3 v3P = P - v3;
    inside_out_test = v31.cross(v3P);
    if ((*v = N.dot(inside_out_test)) < 0)
    {
        return false;
    }

    // 5. calculate UV coords(barycentric coord) for P in v1, v2 ,v3
    *u /= denom;
    *v /= denom;
    
    return true;
}