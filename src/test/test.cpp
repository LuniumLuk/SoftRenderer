#include <stdio.h>
#include <stdlib.h>
#include "../api.hpp"

using namespace Lurdr;

void testFunc(void* out[]) {
    // out[0] = (void*)(new float);
    printf("%lu\n", sizeof(void*));
    delete[] static_cast<char*>(out[0]);
    float* out1 = static_cast<float*>(out[0] = new char[sizeof(float)]);

    *out1 = 2.5f;
}

class A
{
public:
    float * m_buffer;

    A(float ix, float iy)
    {
        m_buffer = new float[2];
        m_buffer[0] = ix;
        m_buffer[1] = iy;
    }

    ~A()
    {
        delete[] m_buffer;
    }

    void operator= (float* input)
    {

    }
};

struct vertex_in
{
    Vector3 position;
    Vector3 normal;
};

struct vertex_out
{
    Vector4 position;
    Vector3 normal;
};

void vertexShaderA(const vertex_in & in, vertex_out & out)
{
    out.position = Vector4(in.position, 1.0f);
    out.normal = in.normal;
};

void testVertexShader(SHADER_PARAM)
{
    Vector3 *a_pos   = layout_in(Vector3, 0);

    Vector4 *position = layout_out(Vector4, 0);
    *position = Vector4(*a_pos, 1.0f);
}

int main()
{

    // Program my_program;
    // my_program.linkShader(VERTEX_SHADER, simpleVertexShader);
    // my_program.linkShader(FRAGMENT_SHADER, simpleFragmentShader);

    // Matrix4 model = Matrix4::IDENTITY;
    // Matrix4 view = Matrix4::IDENTITY;
    // Matrix4 projection = Matrix4::IDENTITY;

    // my_program.setUniform(0, &model);
    // my_program.setUniform(1, &view);
    // my_program.setUniform(2, &projection);

    printf("=== Test 0 ===\n");

    float triangle_vertices[][3] = {
        { 0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
    };
    size_t triangle_indices[][3] = {
        { 1, 2, 3 },
        { 1, 3, 4 },
    };

    FrameBuffer frame_buffer(512, 512);

    VertexArray vertex_array;
    ArrayBuffer array_buffer;
    ElementBuffer element_buffer;
    array_buffer.setBufferData(12, &triangle_vertices[0][0]);
    array_buffer.setDataPointers(0, 3, 3, 0);
    element_buffer.setBufferData(6, &triangle_indices[0][0]);
    vertex_array.bindDataArray(&array_buffer);
    vertex_array.bindIndiciesArray(&element_buffer);

    Program shader_program;
    shader_program.linkShader(VERTEX_SHADER, testVertexShader);

    drawTriangles(frame_buffer, vertex_array, shader_program);

    printf("=== Test 1 ===\n");

    // printf("%lu\n", sizeof(A));
    // printf("%lu\n", sizeof(Vector2));
    // // printf("%f\n", a.x);
    // // printf("%f\n", a.y);

    // void * buffer = new char[sizeof(Vector2)];
    // Vector4 * vec = static_cast<Vector4*>(buffer); // ？？？
    // vec->print();

    // printf("%lu\n", sizeof(int16_t));


    // void * t = new char[sizeof(int16_t)];
    // int32_t * tt = static_cast<int32_t *>(t);

    // *tt = 0xFFFFFFFF;
    // *(tt + 1) = 0xFFFFFFFF;

    // int64_t * ttt = static_cast<int64_t *>((void *)tt);
    // printf("0x%016llx\n", *ttt);

    // printf("=== ==== ===\n");

    // int16_t aaa = 0b0110001111011110;
    // for (int i = 15; i >= 0; i--)
    // {
    //     printf("%d", 1 & (aaa >> i));
    // }
    // printf("\n");

    // FrameBuffer frame_buffer;

    // drawInteger(frame_buffer, 0, 0, 14560, 10, RGBCOLOR(0.0f, 0.0f, 0.0f));

    // void* out[255];
    // out[0] = new char[4];

    // testFunc(out);

    // printf("%f\n", *static_cast<float*>(out[0]));

    return 0;
}