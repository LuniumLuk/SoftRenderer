#include "scene.hpp"

using namespace Lurdr;


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

void Scene::sort_model(const Matrix4 & view_matrix)
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