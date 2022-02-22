#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../global.hpp"

namespace Lurdr
{
    typedef struct APPWINDOW AppWindow;
    typedef enum {KEY_A, KEY_D, KEY_S, KEY_W, KEY_SPACE, KEY_ESCAPE, KEY_NUM} KEY_CODE;
    typedef enum {BUTTON_L, BUTTON_R, BUTTON_NUM} MOUSE_BUTTON;

    struct TIME {
        int year;
        int month;
        int day_of_week;
        int day;
        int hour;
        int minute;
        int second;
        int millisecond;
    };
    typedef struct TIME Time;


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
    void swapBuffer(AppWindow *window);
    bool windowShouldClose(AppWindow *window);

    /**
     *  input & callbacks
     */
    void pollEvent();
    bool isKeyDown(AppWindow *window, KEY_CODE key);
    bool isMouseButtonDown(AppWindow *window, MOUSE_BUTTON button);
    void getMousePosition(AppWindow *window, float *x, float *y);
    void setKeyboardCallback(AppWindow *window, void(*callback)(AppWindow*, KEY_CODE, bool));
    void setMouseButtonCallback(AppWindow *window, void(*callback)(AppWindow*, MOUSE_BUTTON, bool));
    void setMouseScrollCallback(AppWindow *window, void(*callback)(AppWindow*, float));
    void setMouseDragCallback(AppWindow *window, void(*callback)(AppWindow*, float, float));

    /**
     *  time
     */
    Time getSystemTime();
}


#endif