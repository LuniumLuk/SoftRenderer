#include "../include/mesh.h"

using namespace Lurdr;

void OBJMesh::init()
{
    m_filename = nullptr;
    m_vs = nullptr;
    m_fs = nullptr;
    m_fvts = nullptr;
    m_fvns = nullptr;
    m_vns = nullptr;
    m_tex_coords = nullptr;
    m_param_space_vs = nullptr;
    m_lines = nullptr;
    m_vertex_count = 0;
    m_face_count = 0;
    m_has_vertex_normals = false;
    m_has_tex_coords = false;
    m_has_param_space_vertices = false;
    m_has_line_elements = false;
}
OBJMesh::OBJMesh()
{
    init();
}
OBJMesh::OBJMesh(const char* filename)
{
    init();

    size_t f_len = strlen(filename);
    m_filename = new char[f_len + 1];
    strcpy(m_filename, filename);

    loadMesh();
}
OBJMesh::~OBJMesh()
{
    delete[] m_filename;
    delete[] m_vs;
    delete[] m_fs;
    delete[] m_fvts;
    delete[] m_fvns;
    delete[] m_vns;
    delete[] m_tex_coords;
    delete[] m_param_space_vs;
    delete[] m_lines;
}
void OBJMesh::loadMesh()
{
    FILE *fp;
    assert(m_filename != nullptr);
    fp = fopen(m_filename, "rb");
    if (fp == nullptr)
    {
        printf("OBJMesh : mesh file: %s open failed\n", m_filename);
        return;
    }

    char line_buffer[MAX_OBJ_LINE];
    while (fgets(line_buffer, MAX_OBJ_LINE, fp))
    {
        printf("=%s=\n", line_buffer);
    }

    fclose(fp);
}
