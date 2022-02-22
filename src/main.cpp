#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utility>
#include "api.hpp"
#include "platform/platform.hpp"

using namespace Lurdr;

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
void mouseScrollEventCallback(AppWindow *window, float offset);
void mouseDragEventCallback(AppWindow *window, float x, float y);

AppWindow *window;
FrameBuffer frame_buffer(512, 512);

int main() {

    printf("------------ MAIN -------------\n");

    // -----------------------------------------
    BMPImage image("assets/lenna.bmp");
    image.printImageInfo();

    UniformImage uniform_image = UniformImage(image);
    uniform_image.printImageInfo();

    // -----------------------------------------

    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    // window = createWindow(title, 512, 512, frame_buffer.colorBuffer());
    window = createWindow(title, 512, 512, uniform_image.getImageBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    int _fps = 0;
    clock_t last_frame_timestamp = clock();
    clock_t last_fps_update = clock();

    swapBuffer(window);

    while (!windowShouldClose(window))
    {
        last_frame_timestamp = clock();
        
        if (clock() - last_fps_update > CLOCKS_PER_SEC)
        {
            _fps = CLOCKS_PER_SEC / max(1.0f, (clock() - last_frame_timestamp));
            last_fps_update = clock();
        }

        // swapBuffer(window);
        pollEvent();
    }

    terminateApplication();

    printf("------------ EXIT -------------\n");
    return 0;
}

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
{
    printf("keyboard event : key[%d] pressed[%u]\n", key, pressed);
    switch(key) {
        case KEY_ESCAPE:
            destroyWindow(window);
            break;
        case KEY_SPACE:
            Time time = getSystemTime();
            printf("year[%d] month[%d] weekday[%d] day[%d] hour[%d] minute[%d] second[%d] ms[%d]\n", 
                time.year, time.month, time.day_of_week, time.day, time.hour, time.minute, time.second, time.millisecond);
            break;
    }
}
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
{
    __unused_variable(window);
    printf("mouse button event : button[%d] pressed[%u]\n", button, pressed);
}
void mouseScrollEventCallback(AppWindow *window, float offset)
{
    __unused_variable(window);
    printf("mouse scroll event : offset[%.2f]\n", offset);
}
void mouseDragEventCallback(AppWindow *window, float x, float y)
{
    __unused_variable(window);
    printf("mouse drag event x[%.2f] y[%.2f]\n", x, y);
}