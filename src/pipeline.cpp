#include "pipeline.hpp"

using namespace Lurdr;

// #define _FLAT_FILL_TRIANGLE_RASTERIZATION_
#define _BARYCENTRIC_TRIANGLE_RASTERIZATION_

#define WIREFRAME_EPSILON 0.5f

bool Pipeline::wireframe_mode = false;
bool Pipeline::depth_test = true;
bool Pipeline::backface_culling = true;

void Pipeline::draw(const FrameBuffer & frame_buffer, const Scene & scene, const Shader * shader)
{
    // scene.sortEntity();
    frame_buffer.clearDepthBuffer(1.0f);

    const DynamicArray<Entity*>* entities = scene.getEntities();
    for (size_t eidx = 0; eidx < entities->size(); eidx++)
    {
        const Entity *entity = (*entities)[eidx];
        const mat4 mvp_matrix = scene.getCamera().getProjectMatrix() * scene.getCamera().getViewMatrix() * entity->getTransform();

        const TriangleMesh *mesh = entity->getTriangleMesh();
        for (size_t fidx = 0; fidx < mesh->faceCount(); fidx++)
        {
#if 0
            vdata vd0 = TRIANGLE_VDATA(fidx, 0);
            vdata vd1 = TRIANGLE_VDATA(fidx, 1);
            vdata vd2 = TRIANGLE_VDATA(fidx, 2);
            vd0.position.print();
            vd1.position.print();
            vd2.position.print();
            printf("----------------------------------------------\n");
#endif
            // Assembly Stage
            v2f v0 = shader->vert(TRIANGLE_VDATA(fidx, 0), entity, scene);
            v2f v1 = shader->vert(TRIANGLE_VDATA(fidx, 1), entity, scene);
            v2f v2 = shader->vert(TRIANGLE_VDATA(fidx, 2), entity, scene);
#if 0
            v0.position.print();
            v1.position.print();
            v2.position.print();
            printf("----------------------------------------------\n");
#endif
            v0.t_normal = TRIANGLE_TRIANGLE_NORMAL(fidx);
            v1.t_normal = TRIANGLE_TRIANGLE_NORMAL(fidx);
            v2.t_normal = TRIANGLE_TRIANGLE_NORMAL(fidx);

            // Perspective Division
            PERSPECTIVE_DIVIDE(v0.position);
            PERSPECTIVE_DIVIDE(v1.position);
            PERSPECTIVE_DIVIDE(v2.position);
            
            // Triangle Screen Clipping
            if ((v0.position.x < -1.0f && v1.position.x < -1.0f && v2.position.x < -1.0f) ||
                (v0.position.x >  1.0f && v1.position.x >  1.0f && v2.position.x >  1.0f) ||
                (v0.position.y < -1.0f && v1.position.y < -1.0f && v2.position.y < -1.0f) ||
                (v0.position.y >  1.0f && v1.position.y >  1.0f && v2.position.y >  1.0f) ||
                (v0.position.z <  0.0f && v1.position.z <  0.0f && v2.position.z <  0.0f) || // Near/Far Plane Clipping
                (v0.position.z >  1.0f && v1.position.z >  1.0f && v2.position.z >  1.0f))
            {
                continue;
            }

            if (backface_culling) { // Back-face Culling
                vec3 u = vec3(v1.position - v0.position);
                vec3 v = vec3(v2.position - v0.position);
                vec3 face_normal = u.cross(v);

                if (face_normal.z < 0.0f)
                {
                    continue;
                }
            }

#ifdef _BARYCENTRIC_TRIANGLE_RASTERIZATION_
            v0.position.x = SCREEN_MAPPING_X(v0.position.x, frame_buffer);
            v1.position.x = SCREEN_MAPPING_X(v1.position.x, frame_buffer);
            v2.position.x = SCREEN_MAPPING_X(v2.position.x, frame_buffer);
            v0.position.y = SCREEN_MAPPING_Y(v0.position.y, frame_buffer);
            v1.position.y = SCREEN_MAPPING_Y(v1.position.y, frame_buffer);
            v2.position.y = SCREEN_MAPPING_Y(v2.position.y, frame_buffer);

            // Preconpute Affine transform for barycentric determinant computation
            // reference : https://stackoverflow.com/questions/24441631/how-exactly-does-opengl-do-perspectively-correct-linear-interpolation
            const float denom = 1.0f / ((v0.position.x - v2.position.x) * (v1.position.y - v0.position.y) - (v0.position.x - v1.position.x) * (v2.position.y - v0.position.y));
            const vec3 barycentric_d0 = denom * vec3(
                v1.position.y - v2.position.y, v2.position.y - v0.position.y, v0.position.y - v1.position.y
            );
            const vec3 barycentric_d1 = denom * vec3(
                v2.position.x - v1.position.x, v0.position.x - v2.position.x, v1.position.x - v0.position.x
            );
            const vec3 barycentric_0 = denom * vec3(
                v1.position.x * v2.position.y - v2.position.x * v1.position.y,
                v2.position.x * v0.position.y - v0.position.x * v2.position.y,
                v0.position.x * v1.position.y - v1.position.x * v0.position.y
            );
            
            // AABB Bounding Box of Triangle
            long x_min = FTOD(min(v0.position.x, min(v1.position.x, v2.position.x)));
            long x_max = FTOD(max(v0.position.x, max(v1.position.x, v2.position.x)));
            long y_min = FTOD(min(v0.position.y, min(v1.position.y, v2.position.y)));
            long y_max = FTOD(max(v0.position.y, max(v1.position.y, v2.position.y)));

            for (long x = x_min; x < x_max; x++)
            {
                for (long y = y_min; y < y_max; y++)
                {
                    vec4 pos(DTOF(x), DTOF(y), 0.0f, 0.0f);

                    const vec3 barycentric = pos.x * barycentric_d0 + pos.y * barycentric_d1 + barycentric_0;
                    if (barycentric.x < 0.0f || barycentric.y < 0.0f || barycentric.z < 0.0f)
                    {
                        continue;
                    }

                    pos.z = barycentric.dot(vec3(v0.position.z, v1.position.z, v2.position.z));
                    pos.w = barycentric.dot(vec3(v0.position.w, v1.position.w, v2.position.w));
                    
                    // Near/Far Plane Clipping
                    if (pos.z < 0.0f || pos.z > 1.0f)
                    {
                        continue;
                    }

                    const vec3 perspective = (1.0f / pos.w) * barycentric.multiply(vec3(v0.position.w, v1.position.w, v2.position.w));

                    v2f v = v2f(
                        pos,
                        mat3( v0.frag_pos.x, v1.frag_pos.x, v2.frag_pos.x,
                              v0.frag_pos.y, v1.frag_pos.y, v2.frag_pos.y,
                              v0.frag_pos.z, v1.frag_pos.z, v2.frag_pos.z ) * perspective,
                        mat3( v0.normal.x, v1.normal.x, v2.normal.x,
                              v0.normal.y, v1.normal.y, v2.normal.y,
                              v0.normal.z, v1.normal.z, v2.normal.z ) * perspective,
                        mat3( v0.t_normal.x, v1.t_normal.x, v2.t_normal.x,
                              v0.t_normal.y, v1.t_normal.y, v2.t_normal.y,
                              v0.t_normal.z, v1.t_normal.z, v2.t_normal.z ) * perspective,
                        vec2( vec3(v0.texcoord.u, v1.texcoord.u, v2.texcoord.u).dot(perspective),
                              vec3(v0.texcoord.v, v1.texcoord.v, v2.texcoord.v).dot(perspective) )
                    );

                    pixelShaderBarycentric(frame_buffer, v, shader, entity, scene);
                }
            }
#endif

#ifdef _FLAT_FILL_TRIANGLE_RASTERIZATION_
            sortVerticesByY(v0, v1, v2); // v0.position.y <= v1.position.y <= v2.position.y

            // Rasterization Stage
            if (v0.position.y == v1.position.y)
            {
                rasterizeFlatTriangle(frame_buffer, v0, v1, v2, shader, entity, scene);
            }
            else if (v1.position.y == v2.position.y)
            {
                rasterizeFlatTriangle(frame_buffer, v1, v2, v0, shader, entity, scene);
            }
            else
            {
                float alpha = (v1.position.y - v0.position.y) / (v2.position.y - v0.position.y);
                v2f v3 = V2F_LERP_LINEAR(v0, v2, alpha);
                rasterizeFlatTriangle(frame_buffer, v1, v3, v0, shader, entity, scene);
                rasterizeFlatTriangle(frame_buffer, v1, v3, v2, shader, entity, scene);
            }
#endif
        }
    }
}


/**
 * Rasterize a flat triangle as below
 * v0      v1
 * 
 *     v2
 */
void Pipeline::rasterizeFlatTriangle(
    const FrameBuffer & frame_buffer, const v2f & v0, const v2f & v1, const v2f & v2, const Shader * shader,
    const Entity * entity, const Scene & scene
) {
    long dy = v2.position.y > v0.position.y ? 1 : -1;
    long y_start = SCREEN_MAPPING_Y(v0.position.y, frame_buffer) - dy;
    long y_end = SCREEN_MAPPING_Y(v2.position.y, frame_buffer) + dy;
    long y_span = y_end - dy - y_start;

    for (long y = y_start; y != y_end; y += dy)
    {
        if (y < 0 || y >= frame_buffer.getHeight())
        {
            continue;
        }

        float alpha = (float)(y - y_start) / (float)y_span + EPSILON;
        rasterizeScanLine( frame_buffer,
                      V2F_LERP_LINEAR(v0, v2, alpha),
                      V2F_LERP_LINEAR(v1, v2, alpha),
                      shader, entity, scene
        );
    }

}

void Pipeline::rasterizeScanLine(
    const FrameBuffer & frame_buffer, const v2f & v0, const v2f & v1, const Shader * shader,
    const Entity * entity, const Scene & scene
) {
    if ((v0.position.x < -1.0f && v1.position.x < -1.0f) ||
        (v0.position.x >  1.0f && v1.position.x >  1.0f) ||
        (v0.position.y < -1.0f && v1.position.y < -1.0f) ||
        (v0.position.y >  1.0f && v1.position.y >  1.0f) ||
        (v0.position.z <  0.0f && v1.position.z <  0.0f) || // Near/Far Plane Clipping
        (v0.position.z >  1.0f && v1.position.z >  1.0f))
    {
        return;
    }
    
    long dx = v1.position.x > v0.position.x ? 1 : -1;
    long x_start = SCREEN_MAPPING_X(v0.position.x, frame_buffer) - dx;
    long x_end = SCREEN_MAPPING_X(v1.position.x, frame_buffer) + dx;
    long x_span = x_end - dx - x_start;

    for (long x = x_start; x != x_end; x += dx)
    {
        if (x < 0 || x >= frame_buffer.getWidth())
        {
            continue;
        }

        float alpha = (float)(x - x_start) / (float)x_span + EPSILON;
        if (wireframe_mode && (x != x_start) && (x != x_end - dx))
        {
            continue;
        }
        pixelShader( frame_buffer,
                        V2F_LERP_LINEAR(v0, v1, alpha),
                        shader, entity, scene
        );
    }
}

void Pipeline::pixelShaderBarycentric(
    const FrameBuffer & frame_buffer, const v2f & v, const Shader * shader,
    const Entity * entity, const Scene & scene
) {
    // Depth Test
    long x = v.position.x;
    long y = v.position.y;
    if (x < 0 || x >= frame_buffer.getWidth() || y < 0 || y >= frame_buffer.getHeight())
    {
        return;
    }

    long depth_buffer_pos = frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x;
    if (depth_test && frame_buffer.depthBuffer()[depth_buffer_pos] <= v.position.z)
    {
        return;
    }

    // TODO: here we ignore alpha channel
    frame_buffer.depthBuffer()[depth_buffer_pos] = v.position.z;

    // Fragment Shader 
    rgba color = shader->frag(v, entity, scene);

    byte_t *color_buffer = frame_buffer.colorBuffer();
    long color_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 3;
    color_buffer[color_buffer_pos++] = FTOD(color.r * 255);
    color_buffer[color_buffer_pos++] = FTOD(color.g * 255);
    color_buffer[color_buffer_pos] = FTOD(color.b * 255);
}

void Pipeline::pixelShader(
    const FrameBuffer & frame_buffer, const v2f & v, const Shader * shader,
    const Entity * entity, const Scene & scene
) {
    // Near/Far Plane Clipping
    if (v.position.z < 0.0f || v.position.z > 1.0f)
    {
        return;
    }

    // Depth Test
    long x = SCREEN_MAPPING_X(v.position.x, frame_buffer);
    long y = SCREEN_MAPPING_Y(v.position.y, frame_buffer);
    if (x < 0 || x >= frame_buffer.getWidth() || y < 0 || y >= frame_buffer.getHeight())
    {
        return;
    }

    long depth_buffer_pos = frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x;
    if (depth_test && frame_buffer.depthBuffer()[depth_buffer_pos] <= v.position.z)
    {
        return;
    }

    // TODO: here we ignore alpha channel
    frame_buffer.depthBuffer()[depth_buffer_pos] = v.position.z;

    // Fragment Shader 
    rgba color = shader->frag(v, entity, scene);

    byte_t *color_buffer = frame_buffer.colorBuffer();
    long color_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 3;
    color_buffer[color_buffer_pos++] = FTOD(color.r * 255);
    color_buffer[color_buffer_pos++] = FTOD(color.g * 255);
    color_buffer[color_buffer_pos] = FTOD(color.b * 255);
}


void Pipeline::sortVerticesByY(v2f & v0, v2f & v1, v2f & v2)
{
    if (v0.position.y > v1.position.y)
    {
        std::swap(v0, v1);
    }
    if (v1.position.y > v2.position.y)
    {
        std::swap(v1, v2);
        if (v0.position.y > v1.position.y)
        {
            std::swap(v0, v1);
        }
    }
}


void Lurdr::drawTriangles(
    const FrameBuffer & frame_buffer,
    const VertexArray & vertex_array,
    const Program & program )
{
    size_t triangle_count = vertex_array.getTriangleCount();
    size_t data_count = vertex_array.getDataCount();
    size_t i = 0, j = 0, k = 0;

    void* in_data[data_count];
    // buffer to store shader out data of a triangle
    void* out_data[3][MAX_OUT_COUNT];
    // buffer to store shader out data sizes of a triangle
    size_t out_sizes[3][MAX_OUT_COUNT];
    for (j = 0; j < 3; j++)
    {
        // set all sizes to 0
        memset(out_sizes[j], 0, MAX_OUT_COUNT * sizeof(size_t));
    }

    for (i = 0; i < triangle_count; i++)
    {
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < data_count; k++)
            {
                in_data[k] = (void*)vertex_array.getData(i, j, k);
            }
            program.run(VERTEX_SHADER, data_count, in_data, &out_sizes[j][0], &out_data[j][0]);
        }

        for (j = 0; j < 3; j++)
        {
            assert(out_sizes[j][0] == sizeof(Vector4));
        }
        
        Vector2 v1 = Vector2(
            ((Vector4*)out_data[0][0])->x * frame_buffer.getWidth(),
            ((Vector4*)out_data[0][0])->y * frame_buffer.getHeight()
        );
        Vector2 v2 = Vector2(
            ((Vector4*)out_data[1][0])->x * frame_buffer.getWidth(),
            ((Vector4*)out_data[1][0])->y * frame_buffer.getHeight()
        );
        Vector2 v3 = Vector2(
            ((Vector4*)out_data[2][0])->x * frame_buffer.getWidth(),
            ((Vector4*)out_data[2][0])->y * frame_buffer.getHeight()
        );

        drawLine(frame_buffer, v1, v2, COLOR_WHITE);
        drawLine(frame_buffer, v2, v3, COLOR_WHITE);
        drawLine(frame_buffer, v3, v1, COLOR_WHITE);
    }
}