#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utility>
#include "global.hpp"
#include "maths.hpp"
#include "image.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include "darray.hpp"
#include "rasterizer.hpp"
#include "scene.hpp"
#include "platform.hpp"

using namespace Lurdr;

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
void mouseScrollEventCallback(AppWindow *window, float offset);
void mouseDragEventCallback(AppWindow *window, float x, float y);

AppWindow *window;
FrameBuffer frame_buffer(512, 512);
Model model;
Scene scene;
Camera camera;

float rotate_angle = 0.0f;
Vector3 camera_pos;
Vector3 camera_dir;
Vector3 camera_tar;

int main() {
    double duration;
    clock_t start, end;
    start = clock();

#ifdef DEBUG
    printf("-------- MAIN (DEBUG) ---------\n");
#else
    printf("------------ MAIN -------------\n");
#endif

    OBJMesh obj_mesh("assets/bunny.obj");
    UniformMesh uni_mesh(obj_mesh);
    uni_mesh.printMeshInfo();

    camera_tar = uni_mesh.getCenter();
    camera_dir = Vector3::UNIT_Z;
    camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);

    model.addMesh(&uni_mesh);
    model.setTransform(Matrix4::IDENTITY);
    scene.addModel(&model);
    camera.setTransform(camera_pos, camera_tar);
    scene.drawScene(frame_buffer, camera);

    initializeApplication();

    const char * title = "viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    runApplication();

    end = clock();
    duration = (double)(end - start);
    printf("Use Time: %9.3fms\n", (duration * 1000 / CLOCKS_PER_SEC));

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
                rotate_angle -= 0.05f;
                break;
            case KEY_S:
                camera_dir.z += 0.05f;
                break;
            case KEY_D:
                rotate_angle += 0.05f;
                break;
            case KEY_W:
                if (camera_dir.z > 0.05f)
                {
                    camera_dir.z -= 0.05f;
                }
                break;
            case KEY_SPACE:
                printf("Press SPACE\n");
                break;
            default:
                break;
        }
        camera.setTransform(camera_pos, camera_tar);
        scene.drawScene(frame_buffer, camera);
        camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);
        updateView(window);
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