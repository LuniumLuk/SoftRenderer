#include "scene.hpp"

using namespace Lurdr;

Model::Model(): m_mesh(nullptr),
                m_transform(Matrix4::IDENTITY),
                m_opaque(true),
                m_distance(0.0f)
{}

Vector3 Model::getMeshCenter() const
{
    return m_mesh->getCenter();
}

Matrix4 Model::getTransform() const
{
    return m_transform;
}

void Model::setDistance(float dist)
{
    m_distance = dist;
}

void Model::addMesh(UniformMesh * mesh)
{
    m_mesh = mesh;
}
void Model::setTransform(const Matrix4 & transfrom)
{
    m_transform = transfrom;
}
void Model::setOpaque(bool opaque)
{
    m_opaque = opaque;
}

// reference : https://github.com/zauonlok/renderer/blob/master/renderer/tests/test_helper.c
bool Model::compare_distance(Model * const & a, Model * const & b)
{
    if (a->m_opaque && b->m_opaque)
    {
        return a->m_distance < b->m_distance;
    }
    else if (a->m_opaque && !b->m_opaque)
    {
        return false;
    }
    else if (!a->m_opaque && b->m_opaque)
    {
        return true;
    }
    else
    {
        return a->m_distance > b->m_distance;
    }
}

Vector2 screenMapping(const Vector3 & src, const FrameBuffer & frame_buffer)
{
    Vector2 vec(
        (src.x + 1.0f) * frame_buffer.getWidth() / 2.0f,
        (src.y + 1.0f) * frame_buffer.getHeight() / 2.0f
    );
    return vec;
}

void Model::drawByFixedPipeline(const FrameBuffer & frame_buffer, const Camera & camera)
{
    Vertex *mesh_vertices = m_mesh->getVertices();
    Matrix4 object_to_projection_matrix = camera.getProjectMatrix() * camera.getViewMatrix() * m_transform;

    BoundingBox bounding_box = m_mesh->getBoundingBox();
    float min_z = bounding_box.min_z;
    float max_z = bounding_box.max_z;

    COLORMAP_TYPE type = COLORMAP_RDYLGN;

    // draw wireframe  
    for (size_t i = 0; i < m_mesh->getVertexCount(); i += 3)
    {
        Vector4 v1 = object_to_projection_matrix * Vector4(mesh_vertices[i].position, 1.0f);
        Vector4 v2 = object_to_projection_matrix * Vector4(mesh_vertices[i + 1].position, 1.0f);
        Vector4 v3 = object_to_projection_matrix * Vector4(mesh_vertices[i + 2].position, 1.0f);

        Vector3 v1_proj = Vector3(v1.x, v1.y, v1.z) / v1.w;
        Vector3 v2_proj = Vector3(v2.x, v2.y, v2.z) / v2.w;
        Vector3 v3_proj = Vector3(v3.x, v3.y, v3.z) / v3.w;

        Vector2 v1_clip = screenMapping(v1_proj, frame_buffer);
        Vector2 v2_clip = screenMapping(v2_proj, frame_buffer);
        Vector2 v3_clip = screenMapping(v3_proj, frame_buffer);

        RGBColor color1 = getColorMap(mesh_vertices[i].position.z, min_z, max_z, type);
        RGBColor color2 = getColorMap(mesh_vertices[i + 1].position.z, min_z, max_z, type);
        RGBColor color3 = getColorMap(mesh_vertices[i + 2].position.z, min_z, max_z, type);

        drawLine(frame_buffer, v1_clip, v2_clip,
            color1, color2, v1_proj.z, v2_proj.z );
        drawLine(frame_buffer, v2_clip, v3_clip,
            color2, color3, v2_proj.z, v3_proj.z );
        drawLine(frame_buffer, v3_clip, v1_clip,
            color3, color1, v3_proj.z, v1_proj.z );
    }
}

void Model::draw(const FrameBuffer & frame_buffer, const Program & program)
{
    __unused_variable(frame_buffer);
    __unused_variable(program);
    // vertex shader
    Vertex *mesh_vertices = m_mesh->getVertices();
    Vector4 *position_buffer = new Vector4[m_mesh->getVertexCount()];
    
    __unused_variable(mesh_vertices);
    __unused_variable(position_buffer);

    for (size_t i = 0; i < m_mesh->getVertexCount(); i += 3)
    {
        // 
        // program.run(VERTEX_SHADER, 2)
        // mesh_vertices[i].position
    }
}

Scene::Scene(): m_background(COLOR_BLACK) {}

void Scene::addModel(Model * model)
{
    m_models.push_back(model);
}

void Scene::sortModels(const Matrix4 & view_matrix)
{
    size_t model_count = m_models.size();
    if (model_count > 1)
    {
        for (size_t i = 0; i < model_count; i++)
        {
            Model *model = m_models[i];
            Vector3 center = model->getMeshCenter();
            Vector4 model_position(center, 1.0f);
            Vector4 world_position = model->getTransform() * model_position;
            Vector4 view_position = view_matrix * world_position;
            model->setDistance(-view_position.z);
        }
    }
    m_models.sort(Model::compare_distance);
}

void Scene::setBackground(const RGBCOLOR & color)
{
    m_background = color;
}

void Scene::drawSceneByFixedPipeline(const FrameBuffer & frame_buffer, const Camera & camera)
{
    sortModels(camera.getViewMatrix());

    frame_buffer.clearColorBuffer(m_background);
    frame_buffer.clearDepthBuffer(1.0f);

    for (size_t i = 0; i < m_models.size(); i++)
    {
        m_models[i]->drawByFixedPipeline(frame_buffer, camera);
    }
}

void Scene::draw(const FrameBuffer & frame_buffer, const Program & program)
{
    for (size_t i = 0; i < m_models.size(); i++)
    {
        m_models[i]->draw(frame_buffer, program);
    }
}
