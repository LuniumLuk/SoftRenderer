#ifndef __PIPELINE_HPP__
#define __PIPELINE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "rasterizer.hpp"
#include "entity.hpp"
#include "scene.hpp"

namespace Lurdr
{

#define TRIANGLE_VERTEX(fidx,vidx) (mesh->getVertices()[mesh->getFaces()[fidx][vidx]])
#define TRIANGLE_NORMAL(fidx,vidx) (mesh->hasVertexNormals()?mesh->getVertexNormals()[mesh->getFaces()[fidx][vidx]]:vec3::ZERO)
#define TRIANGLE_TEXCOORD(fidx,vidx) (mesh->hasTextureCoords()?mesh->getTextureCoords()[mesh->getFaces()[fidx][vidx]]:vec2::ZERO)

#define TRIANGLE_VDATA(fidx,vidx) { .model_mat = entity->getTransform(),  \
                                    .mvp_mat = mvp_matrix,                \
                                    .position = TRIANGLE_VERTEX(fidx, 0), \
                                    .normal = TRIANGLE_NORMAL(fidx, 0),   \
                                    .texcoord = TRIANGLE_TEXCOORD(fidx, 0) }


class Pipeline
{
public:
    static void draw(const FrameBuffer & frame_buffer, const Scene & scene, const Shader * shader);
};

void drawTriangles(
    const FrameBuffer & frame_buffer,
    const VertexArray & vertex_array,
    const Program & program
);

}

#endif