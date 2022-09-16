#include "pipeline.hpp"

using namespace LuGL;

// #define _FLAT_FILL_TRIANGLE_RASTERIZATION_
// #define _BARYCENTRIC_TRIANGLE_RASTERIZATION_0_
#define _BARYCENTRIC_TRIANGLE_RASTERIZATION_1_

#define WIREFRAME_EPSILON 0.5f

// reference : https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes
static inline float edgeFunction(const vec3 & a, const vec3 & b, const vec3 & c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

static inline bool outsideTest(v2f const& v0, v2f const& v1, v2f const& v2, vec3 const& pos, float* w0, float* w1, float* w2)
{
    *w0 = edgeFunction(v1.position, v2.position, pos);
    *w1 = edgeFunction(v2.position, v0.position, pos);
    *w2 = edgeFunction(v0.position, v1.position, pos);

    bool has_neg = *w0 < 0 || *w1 < 0 || *w2 < 0;
    bool has_pos = *w0 > 0 || *w1 > 0 || *w2 > 0;
    return has_neg && has_pos;
}

void Pipeline::draw(const FrameBuffer & frame_buffer, const Scene & scene, const Shader * shader)
{
    // scene.sortEntity();
    frame_buffer.clearDepthBuffer(1.0f);

    const DynamicArray<Entity*>* entities = scene.getEntities();
    for (size_t eidx = 0; eidx < entities->size(); eidx++)
    {
        const Entity *entity = (*entities)[eidx];
        const mat4 mvp_matrix = scene.getCamera().getProjectMatrix() * scene.getCamera().getViewMatrix() * entity->getTransform();
        const mat3 model_inv_transpose = mat3(entity->getTransform().inversed().transposed());

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
            v0.t_normal = model_inv_transpose * TRIANGLE_TRIANGLE_NORMAL(fidx);
            v1.t_normal = model_inv_transpose * TRIANGLE_TRIANGLE_NORMAL(fidx);
            v2.t_normal = model_inv_transpose * TRIANGLE_TRIANGLE_NORMAL(fidx);

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

            if (Singleton<Global>::get().backface_culling && !Singleton<Global>::get().wireframe_mode) { // Back-face Culling
                vec3 u = vec3(v1.position - v0.position);
                vec3 v = vec3(v2.position - v0.position);
                vec3 face_normal = u.cross(v);

                if (face_normal.z < 0.0f)
                {
                    continue;
                }
            }

#ifdef _BARYCENTRIC_TRIANGLE_RASTERIZATION_0_
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
            long x_min = min(v0.position.x, min(v1.position.x, v2.position.x));
            long x_max = max(v0.position.x, max(v1.position.x, v2.position.x));
            long y_min = min(v0.position.y, min(v1.position.y, v2.position.y));
            long y_max = max(v0.position.y, max(v1.position.y, v2.position.y));

            for (long x = x_min; x < x_max; x++)
            {
                for (long y = y_min; y < y_max; y++)
                {
                    vec4 pos(DTOF(x), DTOF(frame_buffer.getHeight() - y), 0.0f, 0.0f);

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

#ifdef _BARYCENTRIC_TRIANGLE_RASTERIZATION_1_
            v0.position.x = SCREEN_MAPPING_X(v0.position.x, frame_buffer);
            v1.position.x = SCREEN_MAPPING_X(v1.position.x, frame_buffer);
            v2.position.x = SCREEN_MAPPING_X(v2.position.x, frame_buffer);
            v0.position.y = SCREEN_MAPPING_Y(v0.position.y, frame_buffer);
            v1.position.y = SCREEN_MAPPING_Y(v1.position.y, frame_buffer);
            v2.position.y = SCREEN_MAPPING_Y(v2.position.y, frame_buffer);

            v0.position.z = 1.0f / v0.position.z;
            v1.position.z = 1.0f / v1.position.z;
            v2.position.z = 1.0f / v2.position.z;

            if (Singleton<Global>::get().wireframe_mode)
            {
                drawLinePipeline(frame_buffer, v0, v1, shader, entity, scene);
                drawLinePipeline(frame_buffer, v1, v2, shader, entity, scene);
                drawLinePipeline(frame_buffer, v2, v0, shader, entity, scene);

                continue;
            }

            // AABB Bounding Box of Triangle
            const long x_min = max(min(v0.position.x, min(v1.position.x, v2.position.x)), 0);
            const long x_max = min(max(v0.position.x, max(v1.position.x, v2.position.x)), frame_buffer.getWidth() - 1);
            const long y_min = max(min(v0.position.y, min(v1.position.y, v2.position.y)), 0);
            const long y_max = min(max(v0.position.y, max(v1.position.y, v2.position.y)), frame_buffer.getHeight() - 1);

            const float area = edgeFunction(v0.position, v1.position, v2.position);
            unsigned short mask;

            for (long x = x_min; x < x_max; x++)
            {
                for (long y = y_min; y < y_max; y++)
                {
                    vec4 pos(DTOF(x), DTOF(y), 1.0f, 0.0f);

                    float w0, w1, w2;
                    if (Singleton<Global>::get().multisample_antialias) {
                        float tp0, tp1, tp2;
                        /*  4X MSAA pattern
                         *      *               (-0.1,  0.4)
                         *           *          ( 0.4,  0.1)
                         *    *                 (-0.4, -0.1)
                         *         *            ( 0.1, -0.4)
                         */
                        vec4 pos0(pos.x - 0.1f, pos.y + 0.4f, 1.0f, 0.0f);
                        vec4 pos1(pos.x + 0.4f, pos.y + 0.1f, 1.0f, 0.0f);
                        vec4 pos2(pos.x - 0.4f, pos.y - 0.1f, 1.0f, 0.0f);
                        vec4 pos3(pos.x + 0.1f, pos.y - 0.4f, 1.0f, 0.0f);

                        mask = 0;

                        if (!outsideTest(v0, v1, v2, pos0, &tp0, &tp1, &tp2)) mask |= 1;
                        if (!outsideTest(v0, v1, v2, pos1, &tp0, &tp1, &tp2)) mask |= 2;
                        if (!outsideTest(v0, v1, v2, pos2, &tp0, &tp1, &tp2)) mask |= 4;
                        if (!outsideTest(v0, v1, v2, pos3, &tp0, &tp1, &tp2)) mask |= 8;

                        if (mask == 0) continue;

                        outsideTest(v0, v1, v2, pos, &w0, &w1, &w2);
                    } else {
                        if (outsideTest(v0, v1, v2, pos, &w0, &w1, &w2))
                        {
                            continue;
                        }
                    }

                    w0 /= area;
                    w1 /= area;
                    w2 /= area;


                    const float denom = (w0 * v0.position.z + w1 * v1.position.z + w2 * v2.position.z);
                    pos.z = 1.0f / denom;
                    if (isnan(pos.z))
                    {
                        continue;
                    }

                    // pos.w = w0 * v0.position.w + w1 * v1.position.w + w2 * v2.position.w;
                    const vec3 barycentric = (1.0f / (w0 * v0.position.w + w1 * v1.position.w + w2 * v2.position.w)) * vec3(w0 * v0.position.w, w1 * v1.position.w, w2 * v2.position.w);

                    // Near/Far Plane Clipping
                    if (pos.z < 0.0f || pos.z > 0.999f)
                    {
                        continue;
                    }

                    const v2f v(
                        pos,
                        mat3( v0.frag_pos.x, v1.frag_pos.x, v2.frag_pos.x,
                              v0.frag_pos.y, v1.frag_pos.y, v2.frag_pos.y,
                              v0.frag_pos.z, v1.frag_pos.z, v2.frag_pos.z ) * barycentric,
                        mat3( v0.normal.x, v1.normal.x, v2.normal.x,
                              v0.normal.y, v1.normal.y, v2.normal.y,
                              v0.normal.z, v1.normal.z, v2.normal.z ) * barycentric,
                        mat3( v0.t_normal.x, v1.t_normal.x, v2.t_normal.x,
                              v0.t_normal.y, v1.t_normal.y, v2.t_normal.y,
                              v0.t_normal.z, v1.t_normal.z, v2.t_normal.z ) * barycentric,
                        vec2( vec3(v0.texcoord.u, v1.texcoord.u, v2.texcoord.u).dot(barycentric),
                              vec3(v0.texcoord.v, v1.texcoord.v, v2.texcoord.v).dot(barycentric) )
                    );

                    pixelShaderBarycentric(frame_buffer, v, shader, entity, scene, mask);
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

#if 0
    if (scene.getEnvmap())
    {
        float x_center = (float)(frame_buffer.getWidth() - 1) / 2.0f;
        float y_center = (float)(frame_buffer.getHeight() - 1) / 2.0f;
        for (long x = 0; x < frame_buffer.getWidth(); x++)
        {
            for (long y = 0; y < frame_buffer.getHeight(); y++)
            {
                long depth_buffer_pos = frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x;
                if (depth_test && frame_buffer.depthBuffer()[depth_buffer_pos] < 1.0f)
                {
                    continue;
                }

                vec2 sh = vierDir2Spherical(scene.getCamera().getForward());

                rgb color = scene.getEnvmap()->getPixel(
                    sh.theta + (float)(y - y_center) / (frame_buffer.getHeight() - 1) * scene.getCamera().getFOV(),
                    sh.phi + (float)(x - x_center) / (frame_buffer.getWidth() - 1) * scene.getCamera().getFOV()
                );

                byte_t *color_buffer = frame_buffer.colorBuffer();
                long color_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 3;
                color_buffer[color_buffer_pos++] = FLOAT2BYTECOLOR(color.r);
                color_buffer[color_buffer_pos++] = FLOAT2BYTECOLOR(color.g);
                color_buffer[color_buffer_pos] = FLOAT2BYTECOLOR(color.b);
            }
        }
    }
#endif
}

void Pipeline::drawLinePipeline(
    const FrameBuffer & frame_buffer, const v2f & v0, const v2f & v1, const Shader * shader,
    const Entity * entity, const Scene & scene
) {
    long x0 = FTOD(v0.position.x);
    long y0 = FTOD(v0.position.y);
    long x1 = FTOD(v1.position.x);
    long y1 = FTOD(v1.position.y);

    long dx = fabs(x1 - x0);
    long dy = -fabs(y1 - y0);
    long sx = x0 < x1 ? 1 : -1;
    long sy = y0 < y1 ? 1 : -1;
    long err = dx + dy; 
    long e2;

    while (true)
    {
        pixelShaderWireframe(frame_buffer, x0, y0, shader, entity, scene);

        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void Pipeline::pixelShaderWireframe(
    const FrameBuffer & frame_buffer, long x, long y, const Shader * shader,
    const Entity * entity, const Scene & scene
) {
    // Depth Test
    if (x < 0 || x >= frame_buffer.getWidth() || y < 0 || y >= frame_buffer.getHeight())
    {
        return;
    }

    long depth_buffer_pos = frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x;
    frame_buffer.depthBuffer()[depth_buffer_pos] = 0.0f;

    byte_t *color_buffer = frame_buffer.colorBuffer();
    long color_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 3;
    color_buffer[color_buffer_pos++] = FLOAT2BYTECOLOR(1.0f);
    color_buffer[color_buffer_pos++] = FLOAT2BYTECOLOR(1.0f);
    color_buffer[color_buffer_pos] = FLOAT2BYTECOLOR(1.0f);
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
        if (Singleton<Global>::get().wireframe_mode && (x != x_start) && (x != x_end - dx))
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
    const Entity * entity, const Scene & scene, unsigned short mask
) {
    // Depth Test
    long x = v.position.x;
    long y = v.position.y;
    if (x < 0 || x >= frame_buffer.getWidth() || y < 0 || y >= frame_buffer.getHeight())
    {
        return;
    }

    if (Singleton<Global>::get().multisample_antialias)
    {
        if (Singleton<Global>::get().depth_test)
        {
            for (int i = 0; i < 4; i++)
            {
                long msaa_depth_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 4 + i;
                if ((frame_buffer.depthBufferMSAA()[msaa_depth_buffer_pos] <= v.position.z))
                {
                    mask &= (~(1 << i) & 15);
                }
            }
        }
        if ((mask & 15) == 0) return;

        rgba color = shader->frag(v, entity, scene);

        long rgb_sum[3] = { 0, 0, 0 };
        float depth_sum;

        byte_t *msaa_color_buffer = frame_buffer.colorBufferMSAA();
        for (int i = 0; i < 4; i++)
        {
            long msaa_depth_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 4 + i;
            long msaa_color_buffer_pos = ((frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 4 + i) * 3;
            if (mask & (1 << i))
            {
                frame_buffer.depthBufferMSAA()[msaa_depth_buffer_pos] = v.position.z;
                msaa_color_buffer[msaa_color_buffer_pos++] = FLOAT2BYTECOLOR(color.r);
                msaa_color_buffer[msaa_color_buffer_pos++] = FLOAT2BYTECOLOR(color.g);
                msaa_color_buffer[msaa_color_buffer_pos]   = FLOAT2BYTECOLOR(color.b);
            }

            msaa_color_buffer_pos = ((frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 4 + i) * 3;
            rgb_sum[0] += msaa_color_buffer[msaa_color_buffer_pos++];
            rgb_sum[1] += msaa_color_buffer[msaa_color_buffer_pos++];
            rgb_sum[2] += msaa_color_buffer[msaa_color_buffer_pos];
            depth_sum += frame_buffer.depthBufferMSAA()[msaa_depth_buffer_pos];
        }

        long depth_buffer_pos = frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x;
        frame_buffer.depthBuffer()[depth_buffer_pos] = depth_sum / 4;

        byte_t *color_buffer = frame_buffer.colorBuffer();
        long color_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 3;
        color_buffer[color_buffer_pos++] = rgb_sum[0] / 4;
        color_buffer[color_buffer_pos++] = rgb_sum[1] / 4;
        color_buffer[color_buffer_pos]   = rgb_sum[2] / 4;
    }
    else
    {
        long depth_buffer_pos = frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x;
        if (Singleton<Global>::get().depth_test && (frame_buffer.depthBuffer()[depth_buffer_pos] <= v.position.z))
        {
            return;
        }

        // TODO: here we ignore alpha channel
        frame_buffer.depthBuffer()[depth_buffer_pos] = v.position.z;

        // Fragment Shader 
        rgba color = shader->frag(v, entity, scene);

        byte_t *color_buffer = frame_buffer.colorBuffer();
        long color_buffer_pos = (frame_buffer.getSize() - frame_buffer.getWidth() * (y + 1) + x) * 3;
        color_buffer[color_buffer_pos++] = FLOAT2BYTECOLOR(color.r);
        color_buffer[color_buffer_pos++] = FLOAT2BYTECOLOR(color.g);
        color_buffer[color_buffer_pos]   = FLOAT2BYTECOLOR(color.b);
    }
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
    if (Singleton<Global>::get().depth_test && frame_buffer.depthBuffer()[depth_buffer_pos] <= v.position.z)
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


void LuGL::drawTriangles(
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