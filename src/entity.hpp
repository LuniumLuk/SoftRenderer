#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "global.hpp"
#include "maths.hpp"
#include "mesh.hpp"
#include "material.hpp"

namespace LuGL
{

#define MAX_CONF_LINE 256

class EntityConfig
{
public:
    char *mesh_filename;
    char *albedo_map;
    char *diffuse_map;
    char *specular_map;
    char *normal_map;

    EntityConfig() = delete;
    EntityConfig(const char * filename);
    ~EntityConfig();

    void loadFromFile(const char * filename);
};
typedef EntityConfig entityConf;

class Entity
{
private:
    mat4            m_transform;
    float           m_distance;

    Material        *m_material;
    TriangleMesh    *m_mesh;
    bool            m_material_need_delete;
    bool            m_mesh_need_delete;

public:
    Entity();
    Entity(const entityConf & config);
    ~Entity();

    void setTransform(const mat4 & transform) { m_transform = transform; }
    void setTransform(
        const vec3 & translate,
        const Quaternion & rotation,
        const vec3 & scale )
    {
        m_transform = mat4::fromTRS(translate, rotation, scale);
    }
    const mat4 getTransform() const { return m_transform; }

    void setTriangleMesh(TriangleMesh * mesh) { m_mesh = mesh; }
    const TriangleMesh * getTriangleMesh() const { return m_mesh; }
    TriangleMesh * getTriangleMesh() { return m_mesh; }

    void setMaterial(Material * material) { m_material = material; }
    const Material * getMaterial() const { return m_material; }

    void setDistance(float distance) { m_distance = distance; }
    static bool compareDistance(Entity * const & a, Entity * const & b);
};

}

#endif