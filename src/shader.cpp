#include "shader.hpp"

using namespace Lurdr;

void Program::linkShader(SHADER_TYPE type, SHADER_FUNC(shader))
{
    switch (type)
    {
        case VERTEX_SHADER:
            m_vertex_shader = shader;
            break;
        case FRAGMENT_SHADER:
            m_fragment_shader = shader;
            break;
        case GEOMETRY_SHADER:
            m_geometry_shader = shader;
            break;
    }
}

void Program::run(SHADER_TYPE type, int in_count, void* in[], int out_count, void* out[])
{
    switch (type)
    {
        case VERTEX_SHADER:
            m_vertex_shader(in_count, in, m_uniform, out_count, out);
            break;
        case FRAGMENT_SHADER:
            m_fragment_shader(in_count, in, m_uniform, out_count, out);
            break;
        case GEOMETRY_SHADER:
            m_geometry_shader(in_count, in, m_uniform, out_count, out);
            break;
    }
}

void Program::setUniform(int idx, void* uniform)
{
    assert(idx >= 0 && idx < MAX_UNIFORM_COUNT);
    m_uniform[idx] = uniform;
}

void Lurdr::simpleVertexShader(SHADER_PARAM)
{
    Vector3 a_pos = layout_in(Vector3, 0);

    Matrix4 *model      = uniform(Matrix4, 0);
    Matrix4 *view       = uniform(Matrix4, 1);
    Matrix4 *projection = uniform(Matrix4, 2);

    Vector4 *position = layout_out(Vector4, 0);

    *position = (*projection) * (*view) * (*model) * Vector4(a_pos, 1.0f);
}

void Lurdr::simpleFragmentShader(SHADER_PARAM)
{
    Vector4 *frag_color = layout_out(Vector4, 0);

    *frag_color = Vector4(1.0f, 0.5f, 0.2f, 1.0f);
}
