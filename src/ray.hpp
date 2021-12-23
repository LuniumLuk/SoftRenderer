#ifndef __RAY_HPP__
#define __RAY_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"
#include "scene.hpp"

namespace Lurdr
{

class Ray
{
private:
    Vector3 m_origin;
    Vector3 m_direction;
public:
    Ray();
    Ray(const Vector3 & origin, const Vector3 & dir): m_origin(origin),
                                                      m_direction(dir) {}
    Ray(const Ray & ray): m_origin(ray.m_origin),
                          m_direction(ray.m_direction) {}
    ~Ray() {}

    Vector3 & origin();
    Vector3 & direction();

    bool intersectWithTriangle(
        const Vector3 & v1,
        const Vector3 & v2,
        const Vector3 & v3, 
        float * u,
        float * v ) const;
    bool intersectWithModel(
        const Model & model,
        size_t * face_idx,
        float * u,
        float * v ) const;
    bool intersectWithScene(
        const Scene & scene,
        size_t * model_idx,
        size_t * face_idx,
        float * u,
        float * v ) const;
};

}

#endif