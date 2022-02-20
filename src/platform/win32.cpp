#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "platform.hpp"

// reference : http://www.winprog.org/tutorial/simple_window.html

const char g_szClassName[] = "viewer_window_class";

WNDCLASSEX wc;
HWND hwnd;
MSG Msg;
HINSTANCE hInstance;

struct Lurdr::APPWINDOW
{
    HWND        *handle;
    byte_t      *surface;
    bool        keys[KEY_NUM];
    bool        buttons[BUTTON_NUM];
    bool        should_close;
    void        (*keyboardCallback)(AppWindow *window, KEY_CODE key, bool pressed);
    void        (*mouseButtonCallback)(AppWindow *window, MOUSE_BUTTON button, bool pressed);
    void        (*mouseScrollCallback)(AppWindow *window, float offset);
    void        (*mouseDragCallback)(AppWindow *window, float x, float y);
};

Lurdr::APPWINDOW * g_window;

static void handleKeyPress(WPARAM wParam, bool pressed)
{
    static Lurdr::KEY_CODE key;
    // reference : https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    switch(wParam)
    {
        case 0x41: key = Lurdr::KEY_A;      break;
        case 0x44: key = Lurdr::KEY_D;      break;
        case 0x53: key = Lurdr::KEY_S;      break;
        case 0x57: key = Lurdr::KEY_W;      break;
        case 0x20: key = Lurdr::KEY_SPACE;  break;
        case 0x1B: key = Lurdr::KEY_ESCAPE; break;
        default:   key = Lurdr::KEY_NUM;    break;
    }

    if (key < Lurdr::KEY_NUM)
    {
        g_window->keys[key] = pressed;
        if (g_window->keyboardCallback)
        {
            g_window->keyboardCallback(g_window, key, pressed);
        }
    }
}

static void handleMouseDrag(float x, float y)
{
    if (g_window->mouseDragCallback)
    {
        g_window->mouseDragCallback(g_window, x, y);
    }
}

// handling windows procedures
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // reference : https://docs.microsoft.com/en-us/windows/win32/inputdev
    static POINTS pts_mouse;

    switch(msg)
    {
        // handle keyboard input
        case WM_KEYDOWN:
            handleKeyPress(wParam, true);
            break;
        case WM_KEYUP:
            handleKeyPress(wParam, false);
            break;
        // handle mouse input
        case WM_LBUTTONDOWN:
            printf("<Windows> LEFT BUTTON DOWN\n");
            // pts_mouse = MAKEPOINTS(lParam); 
            // printf("Button Pos: [%d, %d]\n", pts_mouse.x, pts_mouse.y);
            break;
        case WM_MOUSEMOVE:
            if (wParam & MK_LBUTTON) // left mouse drag
            {
                pts_mouse = MAKEPOINTS(lParam); 
                handleMouseDrag(pts_mouse.x, pts_mouse.y);
                // printf("Button Pos: [%d, %d]\n", pts_mouse.x, pts_mouse.y);
            }
            break;
        case WM_PAINT:
            break;
            {
                unsigned char temp_bitmap[300];
                memset(temp_bitmap, 0, sizeof(unsigned char) * 300);

                HDC hdc = GetDC(hwnd);
                HDC compatible_dc = CreateCompatibleDC(hdc);
                ReleaseDC(hwnd, hdc);

                BITMAPINFO bmi;
                bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bmi.bmiHeader.biBitCount=24;
                bmi.bmiHeader.biWidth=10;
                bmi.bmiHeader.biHeight=10;
                bmi.bmiHeader.biCompression=BI_RGB;
                bmi.bmiHeader.biClrUsed=0;
                bmi.bmiHeader.biClrImportant=0;
                bmi.bmiHeader.biPlanes=1;
                bmi.bmiHeader.biSizeImage=0;
                HDC winDC=GetDC(hwnd);

                RECT r={0,10,10,10};
                FillRect(winDC,&r,(HBRUSH)GetStockObject(BLACK_BRUSH));

                SetDIBitsToDevice(
                    winDC,
                    0,0,10,10,
                    0,0,0,10,
                    (void*)temp_bitmap,
                    &bmi,
                    DIB_RGB_COLORS
                );


                ReleaseDC(hwnd,winDC);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Lurdr::initializeApplication()
{
    hInstance = GetModuleHandle(NULL);

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }
}

void Lurdr::runApplication()
{
    
}

void Lurdr::terminateApplication()
{
    DestroyWindow(hwnd);
}

Lurdr::AppWindow* Lurdr::createWindow(const char *title, int width, int height, byte_t *surface_buffer)
{
    hwnd = CreateWindowEx(
        0,
        g_szClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_window = new Lurdr::AppWindow();
    g_window->handle = &hwnd;
    g_window->surface = surface_buffer;

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    return g_window;
}

void Lurdr::destroyWindow(AppWindow *window)
{
    DestroyWindow(*(window->handle));
}

void Lurdr::updateView(AppWindow *window)
{

}

bool Lurdr::windowShouldClose(AppWindow *window)
{
    return window->should_close;
}

void Lurdr::pollEvent()
{
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    // return Msg.wParam;
}

/**
 * input & callback registrations
 */
void Lurdr::setKeyboardCallback(AppWindow *window, void(*callback)(AppWindow*, KEY_CODE, bool))
{
    window->keyboardCallback = callback;
}

void Lurdr::setMouseButtonCallback(AppWindow *window, void(*callback)(AppWindow*, MOUSE_BUTTON, bool))
{
    window->mouseButtonCallback = callback;
}

void Lurdr::setMouseScrollCallback(AppWindow *window, void(*callback)(AppWindow*, float))
{
    window->mouseScrollCallback = callback;
}

void Lurdr::setMouseDragCallback(AppWindow *window, void(*callback)(AppWindow*, float, float))
{
    window->mouseDragCallback = callback;
}