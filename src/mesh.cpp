#include "mesh.hpp"

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
    long scanned_items;
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
                if (scanned_items != 3)
                {
                    scanned_items = sscanf(line_buffer, "f %lu//%lu %lu//%lu %lu//%lu", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                    assert(scanned_items == 6);
                    vertex_idxes.push_back(v1);
                    vertex_idxes.push_back(v2);
                    vertex_idxes.push_back(v3);
                    normal_idxes.push_back(vn1);
                    normal_idxes.push_back(vn2);
                    normal_idxes.push_back(vn3);
                }
                else
                {
                    vertex_idxes.push_back(v1);
                    vertex_idxes.push_back(v2);
                    vertex_idxes.push_back(v3);
                }
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
        for (size_t i = 0; i < vertices.size(); i++)
        {
            m_vs[i] = vertices[i];
        }
    }
    if (vertex_idxes.size() > 0 && vertex_idxes.size() % 3 == 0)
    {
        m_face_count = vertex_idxes.size() / 3;
        m_fs = new size_t[vertex_idxes.size()];
        for (size_t i = 0; i < vertex_idxes.size(); i++)
        {
            m_fs[i] = vertex_idxes[i];
        }
    }
    if (texture_idxes.size() > 0 && texture_idxes.size() % 3 == 0)
    {
        // ...
        m_has_tex_coords = true;
    }
    if (normal_idxes.size() > 0 && normal_idxes.size() % 3 == 0)
    {
        // assert(vertex_normals.size() == m_vertex_count);
        m_vns = new Vector3[vertex_normals.size()];
        for (size_t i = 0; i < vertex_normals.size(); i++)
        {
            m_vns[i] = vertex_normals[i];
        }

        assert(normal_idxes.size() / 3 == m_face_count);
        m_fvns = new size_t[normal_idxes.size()];
        for (size_t i = 0; i < normal_idxes.size(); i++)
        {
            m_fvns[i] = normal_idxes[i];
        }
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

/**
 * UniformMesh
 */
UniformMesh::UniformMesh(): m_vertices(nullptr),
                            m_vertex_count(0),
                            m_face_count(0),
                            m_has_tex_coords(false),
                            m_has_vertex_normals(false),
                            m_bounding_box(BoundingBox()) {}

UniformMesh::UniformMesh(const OBJMesh & obj_mesh)
{
    m_face_count = obj_mesh.m_face_count;
    m_vertex_count = obj_mesh.m_vertex_count;
    m_has_tex_coords = obj_mesh.m_has_tex_coords;
    m_has_vertex_normals = obj_mesh.m_has_vertex_normals;
    m_vertices = new Vertex[m_vertex_count];

    Vector3 coords_min(FLOAT_INF,  FLOAT_INF,  FLOAT_INF);
    Vector3 coords_max(-FLOAT_INF, -FLOAT_INF, -FLOAT_INF);

    Vector3 accumulated_position = Vector3::ZERO;
    for (size_t i = 0; i < m_vertex_count; i++)
    {
        Vertex v;
        v.position = obj_mesh.m_vs[obj_mesh.m_fs[i] - 1];
        accumulated_position += v.position;
        if (v.position.x < coords_min.x) coords_min.x = v.position.x;
        if (v.position.y < coords_min.y) coords_min.y = v.position.y;
        if (v.position.z < coords_min.z) coords_min.z = v.position.z;
        if (v.position.x > coords_max.x) coords_max.x = v.position.x;
        if (v.position.y > coords_max.y) coords_max.y = v.position.y;
        if (v.position.z > coords_max.z) coords_max.z = v.position.z;

        if (m_has_tex_coords)
        {
            v.texture = obj_mesh.m_tex_coords[obj_mesh.m_fvts[i] - 1];
        }
        if (m_has_vertex_normals)
        {
            v.normal = obj_mesh.m_vns[obj_mesh.m_fvns[i] - 1];
        }
        m_vertices[i] = v;
    }
    m_mesh_center = accumulated_position / m_vertex_count;
    m_bounding_box = BoundingBox(
        coords_min.x, coords_min.y, coords_min.z,
        coords_max.x, coords_max.y, coords_max.z
    );
}

UniformMesh::UniformMesh(const UniformMesh & uni_mesh)
{
    m_vertex_count = uni_mesh.m_vertex_count;
    m_face_count = uni_mesh.m_face_count;
    m_has_tex_coords = uni_mesh.m_has_tex_coords;
    m_has_vertex_normals = uni_mesh.m_has_vertex_normals;
    m_vertices = new Vertex[m_vertex_count];
    for (size_t i = 0; i < m_vertex_count; i++)
    {
        m_vertices[i] = uni_mesh.m_vertices[i];
    }
    m_mesh_center = uni_mesh.m_mesh_center;
    m_bounding_box = uni_mesh.m_bounding_box;
}

UniformMesh::~UniformMesh()
{
    delete[] m_vertices;
}

Vertex * UniformMesh::getVertices() const
{
    return m_vertices;
}

size_t UniformMesh::getFaceCount() const
{
    return m_face_count;
}

size_t UniformMesh::getVertexCount() const
{
    return m_vertex_count;
}

Vector3 UniformMesh::getCenter() const
{
    return m_mesh_center;
}

BoundingBox UniformMesh::getBoundingBox() const
{
    return m_bounding_box;
}


void UniformMesh::printMeshInfo() const
{
    if (m_vertex_count > 0)
    {
        printf("-- UniformMesh info --------------------------\n");
        printf("  vertex count : %-6lu\n", m_vertex_count);
        printf("    face count : %-6lu\n", m_face_count);
        printf("   mesh center : (%6.2f, %6.2f, %6.2f)\n", m_mesh_center.x, m_mesh_center.y, m_mesh_center.z);
        printf("  bounding box : (%6.2f, %6.2f, %6.2f), (%6.2f, %6.2f, %6.2f)\n", 
            m_bounding_box.min_x, m_bounding_box.min_y, m_bounding_box.min_z,
            m_bounding_box.max_x, m_bounding_box.max_y, m_bounding_box.max_z );
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
        printf("-- UniformMesh unloaded ----------------------\n");
    }
}

/**
 * TriangleMesh
 */

TriangleMesh::TriangleMesh():
    m_vertices(nullptr),
    m_vertex_normals(nullptr),
    m_triangle_normals(nullptr),
    m_texture_coords(nullptr),
    m_faces(nullptr),
    m_face_texcoords(nullptr),
    m_face_normals(nullptr),
    m_mesh_center(vec3::ZERO),
    m_vertex_count(0),
    m_face_count(0),
    m_has_vertex_normals(false),
    m_has_triangle_normals(false),
    m_has_texture_coords(false) {}

TriangleMesh::TriangleMesh(const char * filename):
    TriangleMesh()
{
    FILE *fp;
    assert(filename != nullptr);
    fp = fopen(filename, "rb");
    if (fp == nullptr)
    {
        printf("TriangleMesh : mesh file: %s open failed\n", filename);
        return;
    }

    char line_buffer[MAX_OBJ_LINE];
    DynamicArray<vec3>  vertices;
    DynamicArray<vec3>  vertex_normals;
    DynamicArray<vec3i> faces;
    DynamicArray<vec3i> face_texcoords;
    DynamicArray<vec3i> face_normals;
    DynamicArray<vec2>  texture_coords;

    float x, y, z;
    size_t v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
    long scanned_items;
    while (fgets(line_buffer, MAX_OBJ_LINE, fp))
    {
        if (strncmp(line_buffer, "v ", 2) == 0)
        {
            scanned_items = sscanf(line_buffer, "v %f %f %f", &x, &y, &z);
            assert(scanned_items == 3);
            vertices.push_back(vec3(x, y, z));
        }
        else if (strncmp(line_buffer, "vt ", 3) == 0)
        {
            scanned_items = sscanf(line_buffer, "vt %f %f", &x, &y);
            assert(scanned_items == 2);
            texture_coords.push_back(vec2(x, y));
        }
        else if (strncmp(line_buffer, "vn ", 3) == 0)
        {
            scanned_items = sscanf(line_buffer, "vn %f %f %f", &x, &y, &z);
            assert(scanned_items == 3);
            vertex_normals.push_back(vec3(x, y, z));
        }
        else if (strncmp(line_buffer, "f ", 2) == 0)
        {
            scanned_items = sscanf(line_buffer, "f %lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
            if (scanned_items != 9)
            {
                scanned_items = sscanf(line_buffer, "f %lu %lu %lu", &v1, &v2, &v3);
                if (scanned_items != 3)
                {
                    scanned_items = sscanf(line_buffer, "f %lu//%lu %lu//%lu %lu//%lu", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                    if (scanned_items != 6)
                    {
                        scanned_items = sscanf(line_buffer, "f %lu/%lu %lu/%lu %lu/%lu", &v1, &vt1, &v2, &vt2, &v3, &vt3);
                        vec3i ft;
                        ft[0] = vt1 - 1;
                        ft[1] = vt2 - 1;
                        ft[2] = vt3 - 1;
                        face_texcoords.push_back(ft);
                    }
                    else
                    {
                        vec3i fn;
                        fn[0] = vn1 - 1;
                        fn[1] = vn2 - 1;
                        fn[2] = vn3 - 1;
                        face_normals.push_back(fn);
                    }
                    vec3i f;
                    f[0] = v1 - 1;
                    f[1] = v2 - 1;
                    f[2] = v3 - 1;
                    faces.push_back(f);
                }
                else
                {
                    vec3i f;
                    f[0] = v1 - 1;
                    f[1] = v2 - 1;
                    f[2] = v3 - 1;
                    faces.push_back(f);
                }
            }
            else
            {
                vec3i f;
                f[0] = v1 - 1;
                f[1] = v2 - 1;
                f[2] = v3 - 1;
                faces.push_back(f);
                vec3i ft;
                ft[0] = vt1 - 1;
                ft[1] = vt2 - 1;
                ft[2] = vt3 - 1;
                face_texcoords.push_back(ft);
                vec3i fn;
                fn[0] = vn1 - 1;
                fn[1] = vn2 - 1;
                fn[2] = vn3 - 1;
                face_normals.push_back(fn);
            }
        }
    }

    if (vertices.size() > 0)
    {
        m_vertex_count = vertices.size();
        m_vertices = new vec3[vertices.size()];
        for (size_t i = 0; i < vertices.size(); i++)
        {
            m_vertices[i] = vertices[i];
        }
    }
    if (faces.size() > 0)
    {
        m_face_count = faces.size();
        m_faces = new vec3i[faces.size()];
        for (size_t i = 0; i < faces.size(); i++)
        {
            m_faces[i] = faces[i];
        }
    }
    if (face_texcoords.size() > 0)
    {
        assert(face_texcoords.size() == m_face_count);
        m_face_texcoords = new vec3i[face_texcoords.size()];
        for (size_t i = 0; i < face_texcoords.size(); i++)
        {
            m_face_texcoords[i] = face_texcoords[i];
        }
    }
    if (face_normals.size() > 0)
    {
        assert(face_normals.size() == m_face_count);
        m_face_normals = new vec3i[face_normals.size()];
        for (size_t i = 0; i < face_normals.size(); i++)
        {
            m_face_normals[i] = face_normals[i];
        }
    }
    if (texture_coords.size() > 0)
    {
        // assert(texture_coords.size() == m_vertex_count);
        m_texture_coords = new vec2[texture_coords.size()];
        for (size_t i = 0; i < texture_coords.size(); i++)
        {
            m_texture_coords[i] = texture_coords[i];
        }
        m_has_texture_coords = true;
    }
    if (vertex_normals.size() > 0)
    {
        // assert(vertex_normals.size() == m_vertex_count);
        m_vertex_normals = new vec3[vertex_normals.size()];
        for (size_t i = 0; i < vertex_normals.size(); i++)
        {
            m_vertex_normals[i] = vertex_normals[i];
        }
        m_has_vertex_normals = true;
    }
    fclose(fp);
    computeMeshCenter();
}

TriangleMesh::TriangleMesh(const TriangleMesh & tri_mesh):
    TriangleMesh()
{
    m_vertex_count = tri_mesh.m_vertex_count;
    m_face_count = tri_mesh.m_face_count;
    m_has_vertex_normals = tri_mesh.m_has_vertex_normals;
    m_has_triangle_normals = tri_mesh.m_has_triangle_normals;
    m_has_texture_coords = tri_mesh.m_has_texture_coords;

    if (m_vertex_count > 0)
    {
        m_vertices = new vec3[m_vertex_count];
        for (size_t i = 0; i < m_vertex_count; i++)
        {
            m_vertices[i] = tri_mesh.m_vertices[i];
        }
    }
    if (m_face_count > 0)
    {
        m_faces = new vec3i[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_faces[i] = tri_mesh.m_faces[i];
        }
    }
    if (m_has_vertex_normals)
    {
        m_vertex_normals = new vec3[m_vertex_count];
        for (size_t i = 0; i < m_vertex_count; i++)
        {
            m_vertex_normals[i] = tri_mesh.m_vertex_normals[i];
        }

        m_face_normals = new vec3i[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_face_normals[i] = tri_mesh.m_face_normals[i];
        }
    }
    if (m_has_triangle_normals)
    {
        m_triangle_normals = new vec3[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_triangle_normals[i] = tri_mesh.m_triangle_normals[i];
        }
    }
    if (m_has_texture_coords)
    {
        m_texture_coords = new vec2[m_vertex_count];
        for (size_t i = 0; i < m_vertex_count; i++)
        {
            m_texture_coords[i] = tri_mesh.m_texture_coords[i];
        }

        m_face_texcoords = new vec3i[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_face_texcoords[i] = tri_mesh.m_face_texcoords[i];
        }
    }
    computeMeshCenter();
}

TriangleMesh & TriangleMesh::operator= (const TriangleMesh & tri_mesh)
{
    if (m_vertices)         delete[] m_vertices;
    if (m_vertex_normals)   delete[] m_vertex_normals;
    if (m_triangle_normals) delete[] m_triangle_normals;
    if (m_faces)            delete[] m_faces;
    if (m_texture_coords)   delete[] m_texture_coords;

    m_vertex_count = tri_mesh.m_vertex_count;
    m_face_count = tri_mesh.m_face_count;
    m_has_vertex_normals = tri_mesh.m_has_vertex_normals;
    m_has_triangle_normals = tri_mesh.m_has_triangle_normals;
    m_has_texture_coords = tri_mesh.m_has_texture_coords;

    if (m_vertex_count > 0)
    {
        m_vertices = new vec3[m_vertex_count];
        for (size_t i = 0; i < m_vertex_count; i++)
        {
            m_vertices[i] = tri_mesh.m_vertices[i];
        }
    }
    if (m_face_count > 0)
    {
        m_faces = new vec3i[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_faces[i] = tri_mesh.m_faces[i];
        }
    }
    if (m_has_vertex_normals)
    {
        m_vertex_normals = new vec3[m_vertex_count];
        for (size_t i = 0; i < m_vertex_count; i++)
        {
            m_vertex_normals[i] = tri_mesh.m_vertex_normals[i];
        }

        m_face_normals = new vec3i[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_face_normals[i] = tri_mesh.m_face_normals[i];
        }
    }
    if (m_has_triangle_normals)
    {
        m_triangle_normals = new vec3[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_triangle_normals[i] = tri_mesh.m_triangle_normals[i];
        }
    }
    if (m_has_texture_coords)
    {
        m_texture_coords = new vec2[m_vertex_count];
        for (size_t i = 0; i < m_vertex_count; i++)
        {
            m_texture_coords[i] = tri_mesh.m_texture_coords[i];
        }

        m_face_texcoords = new vec3i[m_face_count];
        for (size_t i = 0; i < m_face_count; i++)
        {
            m_face_texcoords[i] = tri_mesh.m_face_texcoords[i];
        }
    }
    computeMeshCenter();

    return *this;
}

TriangleMesh::~TriangleMesh()
{
    if (m_vertices)         delete[] m_vertices;
    if (m_vertex_normals)   delete[] m_vertex_normals;
    if (m_triangle_normals) delete[] m_triangle_normals;
    if (m_faces)            delete[] m_faces;
    if (m_face_texcoords)   delete[] m_face_texcoords;
    if (m_face_normals)     delete[] m_face_normals;
    if (m_texture_coords)   delete[] m_texture_coords;
}

void TriangleMesh::printMeshInfo() const
{
    if (m_vertex_count > 0)
    {
        printf("-- TriangleMesh info -------------------------\n");
        printf("    vertex count : %-6lu\n", m_vertex_count);
        printf("      face count : %-6lu\n", m_face_count);
        if (m_has_vertex_normals)
            printf("  vertex normals : True\n");
        else
            printf("  vertex normals : False\n");
        if (m_has_triangle_normals)
            printf("triangle normals : True\n");
        else
            printf("triangle normals : False\n");
        if (m_has_texture_coords)
            printf("  texture coords : True\n");
        else
            printf("  texture coords : False\n");
        
        printf("----------------------------------------------\n");
    }
    else
    {
        printf("-- TriangleMesh unloaded ---------------------\n");
    }
}

void TriangleMesh::computeVertexNormals()
{
    if (m_has_vertex_normals) return;
    
    if (!m_has_triangle_normals)
    {
        computeTriangleNormals();
    }

    if (m_face_normals) delete[] m_face_normals;
    m_face_normals = new vec3i[m_face_count];

    DynamicArray<DynamicArray<size_t> > neighbour_faces(m_vertex_count);
    for (size_t fidx = 0; fidx < m_face_count; fidx++)
    {
        neighbour_faces[m_faces[fidx][0]].push_back(fidx);
        neighbour_faces[m_faces[fidx][1]].push_back(fidx);
        neighbour_faces[m_faces[fidx][2]].push_back(fidx);

        m_face_normals[fidx] = m_faces[fidx]; // use same indices as face
    }

    DynamicArray<vec3> vertex_normals;
    for (size_t vidx = 0; vidx < m_vertex_count; vidx++)
    {
        vec3 n;
        for (size_t i = 0; i < neighbour_faces[vidx].size(); i++)
        {
            n += m_triangle_normals[neighbour_faces[vidx][i]];
        }
        vertex_normals.push_back(n.normalized());
    }

    m_vertex_normals = new vec3[vertex_normals.size()];
    for (size_t i = 0; i < vertex_normals.size(); i++)
    {
        m_vertex_normals[i] = vertex_normals[i];
    }

    m_has_vertex_normals = true;
}

void TriangleMesh::computeTriangleNormals()
{
    if (m_has_triangle_normals) return;

    DynamicArray<vec3> triangle_normals;

    for (size_t fidx = 0; fidx < m_face_count; fidx++)
    {
        vec3 u = m_vertices[m_faces[fidx][1]] - m_vertices[m_faces[fidx][0]];
        vec3 v = m_vertices[m_faces[fidx][2]] - m_vertices[m_faces[fidx][0]];

        triangle_normals.push_back(u.cross(v).normalized());
    }

    m_triangle_normals = new vec3[triangle_normals.size()];
    for (size_t i = 0; i < triangle_normals.size(); i++)
    {
        m_triangle_normals[i] = triangle_normals[i];
    }

    m_has_triangle_normals = true;
}

void TriangleMesh::computeMeshCenter()
{
    vec3 center = vec3::ZERO;

    for (size_t vidx = 0; vidx < m_vertex_count; vidx++)
    {
        center += m_vertices[vidx];
    }

    m_mesh_center = center * (1.0f / m_vertex_count);
}

BoundingBox TriangleMesh::getAxisAlignBoundingBox() const
{
    BoundingBox bounding_box(
         FLOAT_INF,  FLOAT_INF,  FLOAT_INF,
        -FLOAT_INF, -FLOAT_INF, -FLOAT_INF
    );

    for (size_t vidx = 0; vidx < m_vertex_count; vidx++)
    {
        bounding_box.min_x = min(m_vertices[vidx].x, bounding_box.min_x);
        bounding_box.min_y = min(m_vertices[vidx].y, bounding_box.min_y);
        bounding_box.min_z = min(m_vertices[vidx].z, bounding_box.min_z);
        bounding_box.max_x = max(m_vertices[vidx].x, bounding_box.max_x);
        bounding_box.max_y = max(m_vertices[vidx].y, bounding_box.max_y);
        bounding_box.max_z = max(m_vertices[vidx].z, bounding_box.max_z);
    }
    
    return bounding_box;
}

vec3 TriangleMesh::getMaxBound() const
{
    vec3 max_bound(-FLOAT_INF, -FLOAT_INF, -FLOAT_INF);

    for (size_t vidx = 0; vidx < m_vertex_count; vidx++)
    {
        max_bound.x = max(m_vertices[vidx].x, max_bound.x);
        max_bound.y = max(m_vertices[vidx].y, max_bound.y);
        max_bound.z = max(m_vertices[vidx].z, max_bound.z);
    }

    return max_bound;
}

vec3 TriangleMesh::getMinBound() const
{
    vec3 min_bound(FLOAT_INF, FLOAT_INF, FLOAT_INF);

    for (size_t vidx = 0; vidx < m_vertex_count; vidx++)
    {
        min_bound.x = max(m_vertices[vidx].x, min_bound.x);
        min_bound.y = max(m_vertices[vidx].y, min_bound.y);
        min_bound.z = max(m_vertices[vidx].z, min_bound.z);
    }

    return min_bound;
}