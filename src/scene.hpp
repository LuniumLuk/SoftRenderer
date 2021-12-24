#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "buffer.hpp"
#include "global.hpp"
#include "maths.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "rasterizer.hpp"

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
    ~Model() {}

    void addMesh(UniformMesh * mesh);
    void setTransform(const Matrix4 & transfrom);
    void setOpaque(bool opaque);
    Vector3 getMeshCenter() const;
    Matrix4 getTransform() const;
    void setDistance(float dist);
    void draw(const FrameBuffer & frame_buffer, const Camera & camera); // add shader program later

    static bool compare_distance(const void* a, const void* b);
};

/**
 * A scene is a collection of models comprising everything that is included in the environment to be rendered. 
 * A scene can also include material descriptions, lighting, and viewing specifications.
 *  -- Real Time Rendering 4th Edition --
 */
class Scene
{
private:
    RGBCOLOR                m_background;
    DynamicArray<Model*>    m_models;
    // float                   m_ambient_intensity;

    /* shadow mapping */
    // FrameBuffer *m_shadow_buffer;
    // texture_t *shadow_map;
public:
    Scene();
    ~Scene() {}

    void addModel(Model * model);
    void sortModels(const Matrix4 & view_matrix);
    void drawScene(const FrameBuffer & frame_buffer, const Camera & camera);

    void setBackground(const RGBCOLOR & color);
};


}

#endif