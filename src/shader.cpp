#include "shader.hpp"

using namespace Lurdr;

void * Lurdr::allocateOutBuffer(const size_t & size, size_t * allocated_sizes, const int & pos, void ** buffer)
{
    if (allocated_sizes[pos] == size)
    {
        return *buffer;
    }
    else if (allocated_sizes[pos] > 0)
    {
        // already allocated, but different size
        delete[] static_cast<char*>(*buffer);
    }
    allocated_sizes[pos] = size;
    *buffer = (void *)new char[size];
    return *buffer;
}

Program::Program()
{
    m_uniform = new void*[MAX_UNIFORM_COUNT];
}

Program::~Program()
{
    delete[] m_uniform;
}

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

void Program::run(SHADER_TYPE type, const size_t & in_count, void* in[], size_t* out_sizes, void* out[]) const
{
    switch (type)
    {
        case VERTEX_SHADER:
            m_vertex_shader(in_count, in, m_uniform, out_sizes, out);
            break;
        case FRAGMENT_SHADER:
            m_fragment_shader(in_count, in, m_uniform, out_sizes, out);
            break;
        case GEOMETRY_SHADER:
            m_geometry_shader(in_count, in, m_uniform, out_sizes, out);
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
    Vector3 *a_pos = layout_in(Vector3, 0);

    Matrix4 *model      = uniform(Matrix4, 0);
    Matrix4 *view       = uniform(Matrix4, 1);
    Matrix4 *projection = uniform(Matrix4, 2);

    Vector4 *position = layout_out(Vector4, 0);

    *position = (*projection) * (*view) * (*model) * Vector4(*a_pos, 1.0f);
}

void Lurdr::simpleFragmentShader(SHADER_PARAM)
{
    unused_in;
    
    unused_uniform;

    Vector4 *frag_color = layout_out(Vector4, 0);

    *frag_color = Vector4(1.0f, 0.5f, 0.2f, 1.0f);
}
