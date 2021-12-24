#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "global.hpp"
#include "maths.hpp"
#include "image.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include "darray.hpp"
#include "rasterizer.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace Lurdr
{
    typedef struct APPWINDOW AppWindow;
    typedef enum {KEY_A, KEY_D, KEY_S, KEY_W, KEY_SPACE, KEY_NUM} KEY_CODE;
    typedef enum {BUTTON_L, BUTTON_R, BUTTON_NUM} MOUSE_BUTTON;

    /**
     *  application
     */
    void initializeApplication();
    void runApplication();
    void terminateApplication();

    /**
     *  window
     */
    AppWindow *createWindow(const char *title, int width, int height, byte_t *surface_buffer);
    void destroyWindow(AppWindow *window);
    void updateView(AppWindow *window);

    /**
     *  input & callbacks
     */
    int isKeyDown(AppWindow *window, KEY_CODE key);
    int isMouseButtonDown(AppWindow *window, MOUSE_BUTTON button);
    bool isMouseDraging(AppWindow *window);
    void getMousePosition(AppWindow *window, float *xpos, float *ypos);
    void setKeyboardCallback(AppWindow *window, void(*callback)(AppWindow*, KEY_CODE, bool));
    void setMouseButtonCallback(AppWindow *window, void(*callback)(AppWindow*, MOUSE_BUTTON, bool));
    void setMouseScrollCallback(AppWindow *window, void(*callback)(AppWindow*, float));
    void setMouseDragCallback(AppWindow *window, void(*callback)(AppWindow*, float, float));

    /**
     *  time
     */
    float getPlatformTime(void);
}


#endif