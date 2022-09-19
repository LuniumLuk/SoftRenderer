#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.hpp"
#include "maths.hpp"

namespace LuGL
{

struct LightingComponent
{
    vec3 diffuse;
    vec3 specular;
};
typedef LightingComponent LightComp;

class Light
{
public:
    vec3 m_position;
    vec3 m_direction;
    vec3 m_diffuse;
    vec3 m_specular;

    Light():
        m_position(vec3::ZERO),
        m_direction(vec3::UNIT_X),
        m_diffuse(vec3::ZERO),
        m_specular(vec3::ZERO) {}
    Light(
        const vec3 & position,
        const vec3 & direction,
        const vec3 & diffuse,
        const vec3 & specular
    ):  m_position(position),
        m_direction(direction.normalized()),
        m_diffuse(diffuse),
        m_specular(specular) {}

    virtual LightComp calcLight(vec3 normal, vec3 frag_pos, vec3 view_dir) = 0;

    void setPosition(const vec3 & position) { m_position = position; }
    void setDirection(const vec3 & direction) { m_direction = direction.normalized(); }
    void setDiffuse(const vec3 & diffuse) { m_diffuse = diffuse; }
    void setSpecular(const vec3 & specular) { m_specular = specular; }

    const vec3 & getPosition() const { return m_position; }
    const vec3 & getDirection() const { return m_direction; }
    const vec3 & getDiffuse() const { return m_diffuse; }
    const vec3 & getSpecular() const { return m_specular; }
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(): Light() {}
    DirectionalLight(
        const vec3 & position,
        const vec3 & direction,
        const vec3 & diffuse,
        const vec3 & specular
    ): Light(
        position, 
        direction,
        diffuse,
        specular) {}
    
    LightComp calcLight(vec3 normal, vec3 frag_pos, vec3 view_dir);
};

class PointLight : public Light
{
public:
    float m_constant;
    float m_linear;
    float m_quadratic;

    PointLight(): Light() {}
    PointLight(
        const vec3 & position,
        const vec3 & direction,
        const vec3 & diffuse,
        const vec3 & specular
    ): Light(
        position, 
        direction,
        diffuse,
        specular),
        m_constant(1.0f),
        m_linear(0.1f),
        m_quadratic(0.01f) {}
        
    LightComp calcLight(vec3 normal, vec3 frag_pos, vec3 view_dir);
};


}

#endif