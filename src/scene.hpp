#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "buffer.hpp"
#include "shader.hpp"
#include "global.hpp"
#include "maths.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "rasterizer.hpp"
#include "entity.hpp"
#include "light.hpp"

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

struct vdata
{
    mat4 model_mat;
    mat4 mvp_mat;
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

struct v2f
{
    vec4 position;
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoord;
};

class Scene;

class Shader
{
public:
    virtual v2f vert(const vdata in, const Entity * entity, const Scene & scene) const = 0;
    virtual vec4 frag(const v2f in, const Entity * entity, const Scene & scene) const = 0;
};

class Scene
{
private:
    vec3                    m_background;
    mutable DynamicArray<Entity*>   m_entities;
    DynamicArray<Light*>    m_lights;
    Camera                  m_camera;

public:
    Scene() {}
    ~Scene() {}

    void addEntity(Entity * entity);
    void addLight(Light * light);
    Camera & getCamera();
    void sortEntity() const;
    void setBackground(const vec3 & color);

    const DynamicArray<Entity*> * getEntities() const { return &m_entities; }

    const Camera& getCamera() const { return m_camera; }

    LightComp getLight(vec3 normal, vec3 frag_pos, vec3 view_dir) const;
};

#define MODEL_MATRIX        (in.model_mat)
#define VIEW_MATRIX         (scene.getCamera().getViewMatrix())
#define PERSPECTIVE_MATRIX  (scene.getCamera().getProjectMatrix())
#define MVP_MATRIX          (in.mvp_mat)

class UnlitShader
{
public:
    virtual v2f vert(const vdata in, const Entity * entity, const Scene & scene) const
    {
        __unused_variable(entity);
        __unused_variable(scene);

        v2f out;
        out.position = MVP_MATRIX * vec4(in.position, 1.0f);
        out.texcoord = in.texcoord;

        return out;
    }

    virtual vec4 frag(const v2f in, const Entity * entity, const Scene & scene) const
    {
        __unused_variable(scene);
        
        return Texture::sampler(entity->getMaterial()->albedo, in.texcoord);
    }
};


}

#endif