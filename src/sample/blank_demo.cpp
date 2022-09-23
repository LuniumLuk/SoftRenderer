#include "sample.hpp"

using namespace LuGL;

static void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
static void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
static void mouseScrollEventCallback(AppWindow *window, float offset);
static void mouseDragEventCallback(AppWindow *window, float x, float y);

static AppWindow *window;
static FrameBuffer frame_buffer(512, 512);

int blank_demo() {
    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    long _fps = 0;
    float _delta = 0;
    FPS_SETUP();
    while (!windowShouldClose(window))
    {
        FPS_UPDATE(_fps, _delta);

        frame_buffer.clearColorBuffer(rgb(0.0f, 0.0f, 0.0f));

        // it is possible that nothing can be seen if frame rate is too high
        // maybe due to buffer swapping in some platform is not that smooth 
        drawString(
            frame_buffer, 10.0f, 10.0f,
            "FPS", 6.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 40.0f, 10.0f,
            _fps, 6.0f, COLOR_RED);

        swapBuffer(window);
        pollEvent();
    }

    terminateApplication();
    return 0;
}

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
{
    __unused_variable(window);
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
            case KEY_ESCAPE:
                break;
            case KEY_SPACE:
                break;
            default:
                return;
        }
    }
}
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
{
    __unused_variable(window);
    __unused_variable(button);
    __unused_variable(pressed);
}
void mouseScrollEventCallback(AppWindow *window, float offset)
{
    __unused_variable(window);
    __unused_variable(offset);
}
void mouseDragEventCallback(AppWindow *window, float x, float y)
{
    __unused_variable(window);
    __unused_variable(x);
    __unused_variable(y);
}