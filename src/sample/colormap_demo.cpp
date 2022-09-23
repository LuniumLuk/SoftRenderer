#include "sample.hpp"

using namespace LuGL;

static void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
static void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
static void mouseScrollEventCallback(AppWindow *window, float offset);
static void mouseDragEventCallback(AppWindow *window, float x, float y);

static AppWindow *window;
static FrameBuffer frame_buffer(512, 512);

unsigned short current_colormap = COLORMAP_PARULA;

const char colormap_names[][32] = {
    "COLORMAP_PARULA",
    "COLORMAP_ACCENT",
    "COLORMAP_BLUES",
    "COLORMAP_BRBG",
    "COLORMAP_BUGN",
    "COLORMAP_BUPU",
    "COLORMAP_CHROMAJS",
    "COLORMAP_DARK2",
    "COLORMAP_GNBU",
    "COLORMAP_WHGNBU",
    "COLORMAP_GNPU",
    "COLORMAP_GREENS",
    "COLORMAP_GREYS",
    "COLORMAP_ORANGES",
    "COLORMAP_ORRD",
    "COLORMAP_PAIRED",
    "COLORMAP_PASTEL1",
    "COLORMAP_PASTEL2",
    "COLORMAP_PIYG",
    "COLORMAP_PRGN",
    "COLORMAP_PUBUGN",
    "COLORMAP_PUBU",
    "COLORMAP_PUOR",
    "COLORMAP_PURD",
    "COLORMAP_PURPLES",
    "COLORMAP_RDBU",
    "COLORMAP_RDGY",
    "COLORMAP_RDPU",
    "COLORMAP_RDYLBU",
    "COLORMAP_RDYLGN",
    "COLORMAP_REDS",
    "COLORMAP_SAND",
    "COLORMAP_SET1",
    "COLORMAP_SET2",
    "COLORMAP_SET3",
    "COLORMAP_SPECTRAL",
    "COLORMAP_WHYLRD",
    "COLORMAP_YLGNBU",
    "COLORMAP_YLGN",
    "COLORMAP_YLORBR",
    "COLORMAP_YLORRD",
    "COLORMAP_YLRD",
};

int colormap_demo() {

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
        frame_buffer.clearDepthBuffer(1.0f);

        for (long i = 100; i < 412; i++)
        {
            for (long j = 100; j < 412; j++)
            {
                drawPixel(
                    frame_buffer, 
                    i, j, 
                    getColorMap((float)(i - 100) / 312, 0.0f, 1.0f, (COLORMAP_TYPE)current_colormap),
                    0.0f
                );
            }
        }

        drawString(
            frame_buffer, 10.0f, 10.0f,
            "FPS", 6.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 40.0f, 10.0f, 
            _fps, 6.0f, COLOR_RED);

        drawString(
            frame_buffer, 10.0f, 30.0f,
            "KEY ESC   ---- QUIT", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 45.0f,
            "KEY SPACE ---- SWITCH COLORMAP", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 320.0f, 45.0f,
            colormap_names[current_colormap], 6.0f, COLOR_RED);

        swapBuffer(window);
        pollEvent();
    }

    terminateApplication();
    return 0;
}

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
{
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
                destroyWindow(window);
                break;
            case KEY_SPACE:
                current_colormap = (current_colormap + 1) % COLORMAP_NUM;
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