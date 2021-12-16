#include "../include/mesh.hpp"

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
    DynamicArray<Vector3> vertices;
    DynamicArray<Vector2> texture_coords;
    DynamicArray<Vector3> vertex_normals;
    DynamicArray<size_t> vertex_idxes;
    DynamicArray<size_t> texture_idxes;
    DynamicArray<size_t> normal_idxes;

    float x, y, z;
    size_t v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
    int scanned_items;
    while (fgets(line_buffer, MAX_OBJ_LINE, fp))
    {
        if (strncmp(line_buffer, "v ", 2) == 0)
        {
            scanned_items = sscanf(line_buffer, "v %f %f %f", &x, &y, &z);
            assert(scanned_items == 3);
            vertices.push_back(Vector3(x, y, z));
        }
        else if (strncmp(line_buffer, "vt ", 3) == 0)
        {
            scanned_items = sscanf(line_buffer, "vt %f %f", &x, &y);
            assert(scanned_items == 2);
            texture_coords.push_back(Vector2(x, y));
        }
        else if (strncmp(line_buffer, "vn ", 3) == 0)
        {
            scanned_items = sscanf(line_buffer, "vn %f %f %f", &x, &y, &z);
            assert(scanned_items == 3);
            vertex_normals.push_back(Vector3(x, y, z));
        }
        else if (strncmp(line_buffer, "f ", 2) == 0)
        {
            scanned_items = sscanf(line_buffer, "f %lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
            if (scanned_items != 9)
            {
                scanned_items = sscanf(line_buffer, "f %lu %lu %lu", &v1, &v2, &v3);
                assert(scanned_items == 3);
                vertex_idxes.push_back(v1);
                vertex_idxes.push_back(v2);
                vertex_idxes.push_back(v3);
            }
            else
            {
                vertex_idxes.push_back(v1);
                vertex_idxes.push_back(v2);
                vertex_idxes.push_back(v3);
                texture_idxes.push_back(vt1);
                texture_idxes.push_back(vt2);
                texture_idxes.push_back(vt3);
                normal_idxes.push_back(vn1);
                normal_idxes.push_back(vn2);
                normal_idxes.push_back(vn3);
            }
        }
    }

    if (vertices.size() > 0)
    {
        m_vertex_count = vertices.size();
        m_vs = new Vector3[vertices.size()];
        memcpy(m_vs, vertices.data(), vertices.size() * sizeof(Vector3));
    }
    if (vertex_idxes.size() > 0 && vertex_idxes.size() % 3 == 0)
    {
        m_face_count = vertex_idxes.size() / 3;
        m_fs = new size_t[vertex_idxes.size()];
        memcpy(m_fs, vertex_idxes.data(), vertex_idxes.size() * sizeof(size_t));
    }
    if (texture_idxes.size() > 0 && texture_idxes.size() % 3 == 0)
    {
        // ...
        m_has_tex_coords = true;
    }
    if (normal_idxes.size() > 0 && normal_idxes.size() % 3 == 0)
    {
        // ...
        m_has_vertex_normals = true;
    }
    fclose(fp);
}
void OBJMesh::printMeshInfo() const
{
    if (m_vertex_count > 0 && m_face_count > 0)
    {
        printf("-- OBJMesh info ------------------------------\n");
        printf("  vertex count : %-6lu\n", m_vertex_count);
        printf("    face count : %-6lu\n", m_face_count);
        if (m_has_tex_coords)
            printf("texture coords : True\n");
        else
            printf("texture coords : False\n");
        if (m_has_vertex_normals)
            printf("vertex normals : True\n");
        else
            printf("vertex normals : False\n");
        printf("----------------------------------------------\n");
    }
    else
    {
        printf("-- OBJMesh unloaded --------------------------\n");
    }
}