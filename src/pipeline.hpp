#ifndef __PIPELINE_HPP__
#define __PIPELINE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.hpp"
#include "shaderf.hpp"
#include "maths.hpp"
#include "buffer.hpp"
#include "rasterizer.hpp"
#include "entity.hpp"
#include "scene.hpp"

namespace LuGL
{

#define PERSPECTIVE_DIVIDE(v) v.w = 1.0f / v.w; \
                              v.x *= v.w;       \
                              v.y *= v.w;       \
                              v.z *= v.w;

#define FLOAT2BYTECOLOR(x) FTOD(clamp(x,0.0f,1.0f)*255)
#define BYTE2FLOATCOLOR(x) DTOF((float)x/255.0f)
                              

#define TRIANGLE_VERTEX(fidx,vidx) (mesh->getVertices()[mesh->getFaces()[fidx][vidx]])
#define TRIANGLE_NORMAL(fidx,vidx) (mesh->hasVertexNormals()?mesh->getVertexNormals()[mesh->getFaceNormals()[fidx][vidx]]:vec3::ZERO)
#define TRIANGLE_TEXCOORD(fidx,vidx) (mesh->hasTextureCoords()?mesh->getTextureCoords()[mesh->getFaceTexcoords()[fidx][vidx]]:vec2::ZERO)
#define TRIANGLE_TRIANGLE_NORMAL(fidx) (mesh->hasTriangleNormals()?mesh->getTriangleNormals()[fidx]:vec3::ZERO)

#define TRIANGLE_VDATA(fidx,vidx) { .model_mat = entity->getTransform(),       \
                                    .model_inv_transpose = model_inv_transpose,\
                                    .mvp_mat = mvp_matrix,                     \
                                    .position = TRIANGLE_VERTEX(fidx, vidx),   \
                                    .normal = TRIANGLE_NORMAL(fidx, vidx),     \
                                    .texcoord = TRIANGLE_TEXCOORD(fidx, vidx), \
                                    .color = vec4::ZERO }

#define SCREEN_MAPPING_X(x,frame_buffer) FTOD((x * 0.5f + 0.5f) * frame_buffer.getWidth())
#define SCREEN_MAPPING_Y(y,frame_buffer) FTOD((y * 0.5f + 0.5f) * frame_buffer.getHeight())
#define V2F_LERP_LINEAR(v0,v1,alpha) v2f( vec4::lerp(v0.position, v1.position, alpha), \
                                          vec3::lerp(v0.frag_pos, v1.frag_pos, alpha), \
                                          vec3::lerp(v0.normal,   v1.normal,   alpha), \
                                          vec3::lerp(v0.t_normal, v1.t_normal, alpha), \
                                          vec2::lerp(v0.texcoord, v1.texcoord, alpha))

class Pipeline
{
public:
    static void draw(const FrameBuffer & frame_buffer, const Scene & scene, const Shader * shader);

private:
    static void pixelShaderBarycentric(
        const FrameBuffer & frame_buffer, const v2f & v, const Shader * shader,
        const Entity * entity, const Scene & scene, unsigned short mask = 0
    );
    static void pixelShaderWireframe(
        const FrameBuffer & frame_buffer, long x, long y, const Shader * shader,
        const Entity * entity, const Scene & scene
    );
    static void pixelShader(
        const FrameBuffer & frame_buffer, const v2f & v, const Shader * shader,
        const Entity * entity, const Scene & scene
    );
    static void rasterizeScanLine(
        const FrameBuffer & frame_buffer, const v2f & v0, const v2f & v1, const Shader * shader,
        const Entity * entity, const Scene & scene
    );
    static void rasterizeFlatTriangle(
        const FrameBuffer & frame_buffer, const v2f & v0, const v2f & v1, const v2f & v2, const Shader * shader,
        const Entity * entity, const Scene & scene
    );
    static void sortVerticesByY(v2f & v0, v2f & v1, v2f & v2);
    static void drawLinePipeline(const FrameBuffer & frame_buffer, const v2f & v0, const v2f & v1, const Shader * shader,
        const Entity * entity, const Scene & scene);
};

void drawTriangles(
    const FrameBuffer & frame_buffer,
    const VertexArray & vertex_array,
    const Program & program
);

}

#endif