#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "global.hpp"
#include "maths.hpp"
#include "mesh.hpp"

namespace Lurdr
{

class Entity
{
private:
    mat4 m_transform;

    // Material *m_material;
    TriangleMesh *m_mesh;

public:
    Entity();
    Entity(const Entity & entity);
    ~Entity();

    void setTransform(const mat4 & transform) { m_transform = transform; }
    void setTransform(
        const vec3 & translate,
        const Quaternion & rotation,
        const vec3 & scale )
    {
        m_transform = rotation * mat4(
            scale.x, 0.0f, 0.0f, translate.x,
            0.0f, scale.y, 0.0f, translate.y,
            0.0f, 0.0f, scale.z, translate.z,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }
    const mat4 getTransform() const { return m_transform; }

    void setTriangleMesh(TriangleMesh * mesh) { m_mesh = mesh; }
    const TriangleMesh * getTriangleMesh() const { return m_mesh; }
}

}

#endif