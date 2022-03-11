#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "maths.hpp"
#include "image.hpp"
#include "buffer.hpp"

namespace Lurdr
{

#define MAX_UNIFORM_COUNT 64
#define MAX_OUT_COUNT 64

#define SHADER_FUNC(name) void(*name)(long,void**,void**,size_t*,void**)
#define SHADER_PARAM    long in_count, void* in[], void* uniform[], size_t* out_sizes, void* out[]

#define layout_in(T,b)  static_cast<T*>(in[b]);assert(b<in_count)
#define uniform(T,b)    static_cast<T*>(uniform[b]);assert(b<MAX_UNIFORM_COUNT)
#define layout_out(T,b) static_cast<T*>(allocateOutBuffer(sizeof(T),out_sizes,b,&out[b]))

#define unused_in       (void)in;(void)in_count
#define unused_uniform  (void)uniform
#define unused_out      (void)out;(void)out_sizes

void * allocateOutBuffer(const size_t & size, size_t * allocated_sizes, const long & pos, void ** buffer);

enum SHADER_TYPE
{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER
};

class Program
{
private:
    SHADER_FUNC(m_vertex_shader);
    SHADER_FUNC(m_fragment_shader);
    SHADER_FUNC(m_geometry_shader);

    void**  m_uniform;

public:
    Program();
    ~Program();

    void linkShader(SHADER_TYPE type, SHADER_FUNC(shader));
    void run(SHADER_TYPE type, const size_t & in_count, void* in[], size_t* out_sizes, void* out[]) const;
    void setUniform(long idx, void* uniform);
};

void simpleVertexShader(SHADER_PARAM);
void simpleFragmentShader(SHADER_PARAM);

#define SHADER_UNIFORM      void** uniform_standard, void** uniform_light, void** unifrom_custom

#define MODEL_MATRIX        (*(mat4*)uniform_standard[0])
#define VIEW_MATRIX         (*(mat4*)uniform_standard[1])
#define PERSPECTIVE_MATRIX  (*(mat4*)uniform_standard[2])
#define MVP_MATRIX          (MODEL_MATRIX * VIEW_MATRIX * PERSPECTIVE_MATRIX)

struct vdata
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

class v2f
{
public:
    vec4 position;
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoord;
};

}

#endif