#include "pipeline.hpp"

using namespace Lurdr;

void Lurdr::drawTriangles(
    const FrameBuffer & frame_buffer,
    const VertexArray & vertex_array,
    const Program & program )
{
    size_t triangle_count = vertex_array.getTriangleCount();
    size_t data_count = vertex_array.getDataCount();
    size_t i, j;
    void* in_data[data_count];
    void* out_data[MAX_OUT_COUNT];
    size_t out_sizes[MAX_OUT_COUNT];
    memset(out_sizes, 0, MAX_OUT_COUNT * sizeof(size_t));
    for (i = 0; i < triangle_count; i++)
    {
        // vertex 0
        for (j = 0; j < data_count; j++)
        {
            in_data[j] = (void*)vertex_array.getData(i, 0, j);
        }
        program.run(VERTEX_SHADER, data_count, in_data, out_sizes, out_data);
    }
}