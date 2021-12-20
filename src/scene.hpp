#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "image.hpp"
#include "buffer.hpp"
#include "global.hpp"
#include "maths.hpp"
#include "camera.hpp"
#include "mesh.hpp"

namespace Lurdr
{

class Model
{
private:
    UniformMesh *m_mesh;
    Matrix4     m_transform;
    // for sorting, reference : https://github.com/zauonlok/renderer/blob/master/renderer/core/scene.h
    float       m_opaque;
    float       m_distance;
    // Program     m_program; // shader programs
public:
    Model();
    ~Model();

    Vector3 getMeshCenter() const;
    Matrix4 getTransform() const;
    void setDistance(float dist);
    void draw(const FrameBuffer & frame_buffer); // add shader program here

    static bool compare_distance(const void* a, const void* b);
};

class Scene
{
private:
    RGBCOLOR                m_background;
    DynamicArray<Model*>    m_models;
    float                   m_ambient_intensity;

    /* shadow mapping */
    FrameBuffer *shadow_buffer;
    // texture_t *shadow_map;
public:
    Scene();
    ~Scene();

    void sort_model(const Matrix4 & view_matrix);
};


}

#endif