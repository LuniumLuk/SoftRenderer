#include "pipeline.hpp"

using namespace Lurdr;

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

void Pipeline::draw(const FrameBuffer & frame_buffer, const Scene & scene, const Shader * shader)
{
    __unused_variable(frame_buffer);
    // scene.sortEntity();

    const DynamicArray<Entity*>* entities = scene.getEntities();
    for (size_t eidx = 0; eidx < entities->size(); eidx++)
    {
        const Entity *entity = (*entities)[eidx];
        const mat4 mvp_matrix = entity->getTransform() * scene.getCamera().getViewMatrix() * scene.getCamera().getProjectMatrix();

        int count = 0;

        const TriangleMesh *mesh = entity->getTriangleMesh();
        for (size_t fidx = 0; fidx < mesh->faceCount(); fidx++)
        {
            v2f v0 = shader->vert(TRIANGLE_VDATA(fidx, 0), entity, scene);
            v2f v1 = shader->vert(TRIANGLE_VDATA(fidx, 1), entity, scene);
            v2f v2 = shader->vert(TRIANGLE_VDATA(fidx, 2), entity, scene);

            if ((v0.position.x <= -1.0f && v1.position.x <= -1.0f && v2.position.x <= -1.0f) ||
                (v0.position.x >=  1.0f && v1.position.x >=  1.0f && v2.position.x >=  1.0f) ||
                (v0.position.y <= -1.0f && v1.position.y <= -1.0f && v2.position.y <= -1.0f) ||
                (v0.position.y >=  1.0f && v1.position.y >=  1.0f && v2.position.y >=  1.0f))
            {
                continue;
            }

            vec3 u = v1.position - v0.position;
            vec3 v = v2.position - v0.position;
            vec3 face_normal = u.cross(v);

            if (face_normal.z >= 0.0f)
            {
                continue;
            }
            count++;
        }
        printf("valid faces : %d\n", count);
    }
}
