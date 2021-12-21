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
bool Model::compare_distance(const void* a, const void* b)
{
    Model* model_a = (Model*)a;
    Model* model_b = (Model*)b;
    if (model_a->m_opaque && model_b->m_opaque)
    {
        return model_a->m_distance < model_b->m_distance;
    }
    else if (model_a->m_opaque && !model_b->m_opaque)
    {
        return false;
    }
    else if (!model_a->m_opaque && model_b->m_opaque)
    {
        return true;
    }
    else
    {
        return model_a->m_distance > model_b->m_distance;
    }
}

void Model::draw(const FrameBuffer & frame_buffer, const Camera & camera)
{
    Vertex *mesh_vertices = m_mesh->getVertices();
    Matrix4 object_to_projection_matrix = camera.getProjectMatrix() * camera.getViewMatrix() * m_transform;
    // Matrix4 object_to_projection_matrix = camera.getViewMatrix() * m_transform;
    // draw wireframe

    object_to_projection_matrix.print();
    for (size_t i = 0; i < m_mesh->getVertexCount(); i += 3)
    {
        Vector4 v1 = object_to_projection_matrix * Vector4(mesh_vertices[i].position, 1.0f);
        Vector4 v2 = object_to_projection_matrix * Vector4(mesh_vertices[i + 1].position, 1.0f);
        Vector4 v3 = object_to_projection_matrix * Vector4(mesh_vertices[i + 2].position, 1.0f);

        Vector2 v1_proj = Vector2(v1.x, v1.y) * 256.0f / v1.w;
        Vector2 v2_proj = Vector2(v2.x, v2.y) * 256.0f / v1.w;
        Vector2 v3_proj = Vector2(v3.x, v3.y) * 256.0f / v1.w;

        v1_proj.print();
        v2_proj.print();
        v3_proj.print();
        RGBCOLOR color(255 * i / m_mesh->getFaceCount(), 255, 0);
        drawLine(frame_buffer, v1_proj, v2_proj, color);
        drawLine(frame_buffer, v2_proj, v3_proj, color);
        drawLine(frame_buffer, v3_proj, v1_proj, color);
    }
}

Scene::Scene(): m_background(RGBCOLOR(0, 0, 0)) {}

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

void Scene::drawScene(const FrameBuffer & frame_buffer, const Camera & camera)
{
    sortModels(camera.getViewMatrix());

    frame_buffer.clearColorBuffer(m_background);
    frame_buffer.clearDepthBuffer(1.0f);

    for (size_t i = 0; i < m_models.size(); i++)
    {
        m_models[i]->draw(frame_buffer, camera);
    }
}