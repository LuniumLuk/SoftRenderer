#ifndef __MESH_H__
#define __MESH_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "global.h"
#include "maths.h"

namespace Lurdr {

#define MAX_OBJ_LINE 256

class OBJMesh
{
private:
    char    *m_filename;

    vec3    *m_vs;
    ulong_t *m_fs;
    ulong_t *m_fvts;
    ulong_t *m_fvns;
    vec3    *m_vns;
    vec2    *m_tex_coords;
    vec2    *m_param_space_vs;  // parameter space vertices
    ulong_t *m_lines;           // line elements (not support)

    ulong_t  m_vertex_count;
    ulong_t  m_face_count;
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
};

struct Vertex
{
    vec3 position;
    vec2 tex_coord;
    vec3 normal;
    vec4 tangent;
    vec4 joint;     // for animation
    vec4 weight;    // for blend skinning
};
typedef struct Vertex vertex;


}

#endif