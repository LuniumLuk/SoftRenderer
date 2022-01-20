#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utility>
#include "../platform.hpp"
#include "../api.hpp"

using namespace Lurdr;

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
void mouseScrollEventCallback(AppWindow *window, float offset);
void mouseDragEventCallback(AppWindow *window, float x, float y);

AppWindow *window;
FrameBuffer frame_buffer(512, 512);

void testVertexShader(SHADER_PARAM)
{
    Vector3 *a_pos   = layout_in(Vector3, 0);

    Vector4 *position = layout_out(Vector4, 0);
    *position = Vector4(*a_pos, 1.0f);
}

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

int main() {
    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    // -------------- TEST : SHADER --------------

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

    // -------------------------------------------

    int _fps = 0;
    clock_t last_frame_timestamp = clock();
    clock_t last_fps_update = clock();
    while (!windowShouldClose(window))
    {
        last_frame_timestamp = clock();
        // printf("FPS: %d\n", _fps);

        if (clock() - last_fps_update > CLOCKS_PER_SEC)
        {
            _fps = CLOCKS_PER_SEC / (clock() - last_frame_timestamp);
            last_fps_update = clock();
        }

        drawTriangles(frame_buffer, vertex_array, shader_program);

        drawString(
            frame_buffer, 10.0f, 35.0f,
            "FPS", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawInteger(
            frame_buffer, 60.0f, 35.0f, 
            _fps, 10.0f, RGBColor(255.0f, 0.0f, 0.0f));

        updateView(window);
        pollEvent();
    }

    return 0;
}

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
{
    assert(window);
    if (pressed)
    {
        switch (key)
        {
            case KEY_A:
                break;
            case KEY_S:
                break;
            case KEY_D:
                break;
            case KEY_W:
                break;
            case KEY_SPACE:
                break;
            case KEY_ESCAPE:
                break;
            default:
                return;
        }
    }
}
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
{
    assert(window);
    printf("mouseButtonEventCallback : %p %d %u\n", window, button, pressed);
}
void mouseScrollEventCallback(AppWindow *window, float offset)
{
    assert(window);
    printf("mouseScrollEventCallback : %p %f\n", window, offset);
}
void mouseDragEventCallback(AppWindow *window, float x, float y)
{
    assert(window);
    printf("mouseDragEventCallback : %p %f %f\n", window, x, y);
}