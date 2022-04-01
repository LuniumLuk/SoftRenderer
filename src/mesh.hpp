#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "global.hpp"
#include "maths.hpp"
#include "darray.hpp"

namespace Lurdr
{

#define MAX_OBJ_LINE 256
#define FLOAT_INF 1e6

struct BoundingBox
{
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;

    BoundingBox()
    {
        min_x = 0.0f;
        min_y = 0.0f;
        min_z = 0.0f;
        max_x = 0.0f;
        max_y = 0.0f;
        max_z = 0.0f;
    }
    BoundingBox(float _min_x, float _min_y, float _min_z, float _max_x, float _max_y, float _max_z)
    {
        // assert(_min_x <= _max_x);
        // assert(_min_y <= _max_y);
        // assert(_min_z <= _max_z);
        min_x = _min_x;
        min_y = _min_y;
        min_z = _min_z;
        max_x = _max_x;
        max_y = _max_y;
        max_z = _max_z;
    }
};

class OBJMesh
{
private:
    char    *m_filename;

    vec3    *m_vs;
    size_t  *m_fs;
    size_t  *m_fvts;
    size_t  *m_fvns;
    vec3    *m_vns;
    vec2    *m_tex_coords;
    vec2    *m_param_space_vs;  // parameter space vertices
    size_t  *m_lines;           // line elements (not support)

    size_t   m_vertex_count;
    size_t   m_face_count;
    bool     m_has_vertex_normals;
    bool     m_has_tex_coords;
    bool     m_has_param_space_vertices;
    bool     m_has_line_elements;

    void loadMesh();
    void init();
public:
    OBJMesh();
    OBJMesh(const char* filename);
    OBJMesh(const OBJMesh & mesh);
    ~OBJMesh();

    void printMeshInfo() const;

    friend class UniformMesh;
};

struct Vertex
{
    vec3 position;
    vec2 texture;
    vec3 normal;
    vec4 tangent;
    vec4 joint;     // for animation
    vec4 weight;    // for blend skinning
};
typedef struct Vertex vertex;

class UniformMesh
{
private:
    Vertex  *m_vertices;
    size_t  m_vertex_count;
    size_t  m_face_count;
    bool    m_has_tex_coords;
    bool    m_has_vertex_normals;
    Vector3 m_mesh_center;
    BoundingBox m_bounding_box;
public:
    UniformMesh();
    UniformMesh(const OBJMesh & obj_mesh);
    UniformMesh(const UniformMesh & uni_mesh);
    ~UniformMesh();

    Vertex * getVertices() const;
    size_t getFaceCount() const;
    size_t getVertexCount() const;
    void printMeshInfo() const;
    Vector3 getCenter() const;
    BoundingBox getBoundingBox() const;
};

#define VERTEX(a) (*(vec3*)&m_vertices[a*3])

class TriangleMesh
{
private:
    vec3    *m_vertices;
    vec3    *m_vertex_normals;
    vec3    *m_triangle_normals;
    vec2    *m_texture_coords;
    vec3i   *m_faces;
    vec3i   *m_face_texcoords;
    vec3i   *m_face_normals;
    vec3    m_mesh_center;

    size_t   m_vertex_count;
    size_t   m_face_count;
    
    bool     m_has_vertex_normals;
    bool     m_has_triangle_normals;
    bool     m_has_texture_coords;
public:
    TriangleMesh();
    TriangleMesh(const char * filename);
    TriangleMesh(const TriangleMesh & tri_mesh);
    ~TriangleMesh();

    TriangleMesh & operator= (const TriangleMesh & tri_mesh);

    void computeVertexNormals();
    void computeTriangleNormals();
    void computeMeshCenter();

    BoundingBox getAxisAlignBoundingBox() const;
    vec3 getMaxBound() const;
    vec3 getMinBound() const;

    bool hasVertexNormals() const { return m_has_vertex_normals; }
    bool hasTriangleNormals() const { return m_has_triangle_normals; }
    bool hasTextureCoords() const { return m_has_texture_coords; }

    size_t vertexCount() const { return m_vertex_count; }
    size_t faceCount() const { return m_face_count; }

    vec3* getVertices() const { return m_vertices; }
    vec3* getVertexNormals() const { return m_vertex_normals; }
    vec3* getTriangleNormals() const { return m_triangle_normals; }
    vec3i* getFaces() const { return m_faces; }
    vec3i* getFaceTexcoords() const { return m_face_texcoords; }
    vec3i* getFaceNormals() const { return m_face_normals; }
    vec2* getTextureCoords() const { return m_texture_coords; }
    vec3 getMeshCenter() const { return m_mesh_center; }

    void printMeshInfo() const;
};

}

#endif