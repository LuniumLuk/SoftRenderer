#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "buffer.hpp"
#include "shaderf.hpp"
#include "global.hpp"
#include "maths.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "rasterizer.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "shader.hpp"
#include "envmap.hpp"

namespace LuGL
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
    void draw(const FrameBuffer & frame_buffer, const Program & program);
    void drawByFixedPipeline(const FrameBuffer & frame_buffer, const Camera & camera); // add shader program later

    static bool compareDistance(Model * const & a, Model * const & b);
};

/**
 * A scene is a collection of models comprising everything that is included in the environment to be rendered. 
 * A scene can also include material descriptions, lighting, and viewing specifications.
 *  -- Real Time Rendering 4th Edition --
 */
class OldScene
{
private:
    RGBCOLOR                m_background;
    DynamicArray<Model*>    m_models;
    // float                   m_ambient_intensity;

    /* shadow mapping */
    // FrameBuffer *m_shadow_buffer;
    // texture_t *shadow_map;
public:
    OldScene();
    ~OldScene() {}

    void addModel(Model * model);
    void sortModels(const Matrix4 & view_matrix);
    void drawSceneByFixedPipeline(const FrameBuffer & frame_buffer, const Camera & camera);
    void draw(const FrameBuffer & frame_buffer, const Program & program);

    void setBackground(const RGBCOLOR & color);
};

class Scene
{
private:
    vec3                    m_background;
    mutable DynamicArray<Entity*>   m_entities;
    DynamicArray<Light*>    m_lights;
    Camera                  m_camera;
    Envmap*                 m_envmap;

public:
    Scene()
    {
        m_envmap = nullptr;
    }
    ~Scene() {}

    void addEntity(Entity * entity);
    void setEnvmap(Envmap * envmap);
    void addLight(Light * light);
    Camera & getCamera();
    void sortEntity() const;
    void setBackground(const vec3 & color);

    const DynamicArray<Entity*> * getEntities() const { return &m_entities; }
    const DynamicArray<Light*> *  getLights() const { return &m_lights; }

    const Camera& getCamera() const { return m_camera; }
    const Envmap* getEnvmap() const { return m_envmap; }

    LightComp calcLight(vec3 normal, vec3 frag_pos, vec3 view_dir) const;
};


}

#endif