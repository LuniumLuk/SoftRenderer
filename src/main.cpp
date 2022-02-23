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

Model model;
Scene scene;
Camera camera;

float rotate_angle = PI;
Vector3 camera_pos;
Vector3 camera_dir;
Vector3 camera_tar;

int main() {

    printf("------------ MAIN -------------\n");

    OBJMesh obj_mesh("assets/simple.obj");
    UniformMesh uni_mesh(obj_mesh);

    TriangleMesh tri_mesh("assets/armadillo.obj");
    {
        Timer t("compute triangle normals");
        tri_mesh.computeTriangleNormals();
    }
    {
        Timer t("compute vertex normals");
        tri_mesh.computeVertexNormals();
    }
    {
        Timer t("compute bounding box");
        BoundingBox bbox = tri_mesh.getAxisAlignBoundingBox();
        printf("%f %f %f %f %f %f\n", bbox.min_x, bbox.min_y, bbox.min_z, bbox.max_x, bbox.max_y, bbox.max_z);
    }
    tri_mesh.printMeshInfo();

    vec3 translate(1.0f, 2.0f, 3.0f);
    vec3 scale(4.4f, 5.5f, 1.2f);
    Quaternion rotation = Quaternion::fromEulerAngles(vec3(1.4f, 2.2f, 0.62f));
    mat4 transform = mat4(
        scale.x, 0.0f, 0.0f, translate.x,
        0.0f, scale.y, 0.0f, translate.y,
        0.0f, 0.0f, scale.z, translate.z,
        0.0f, 0.0f, 0.0f,    1.0f
    ).rotated(rotation);
    transform.print();

    exit(0);

    camera_tar = uni_mesh.getCenter() + Vector3(0.0f, -0.2f, 0.0f);
    camera_dir = Vector3(0.0f, 0.0f, 2.0f);
    camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);

    model.addMesh(&uni_mesh);
    Matrix4 model_transform = Matrix4::IDENTITY;
    model_transform.scale(Vector3(0.5f, 0.5f, 0.5f));
    model.setTransform(model_transform);
    scene.addModel(&model);
    camera.setTransform(camera_pos, camera_tar);

    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    int _fps = 0;
    clock_t last_frame_timestamp = clock();
    clock_t last_fps_update = clock();
    while (!windowShouldClose(window))
    {
        last_frame_timestamp = clock();
        
        if (clock() - last_fps_update > CLOCKS_PER_SEC)
        {
            _fps = CLOCKS_PER_SEC / max(1.0f, (clock() - last_frame_timestamp));
            last_fps_update = clock();
        }

        scene.drawSceneByFixedPipeline(frame_buffer, camera);

        drawString(
            frame_buffer, 10.0f, 10.0f,
            "LU RENDERER", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawString(
            frame_buffer, 10.0f, 35.0f,
            "FPS", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawInteger(
            frame_buffer, 60.0f, 35.0f, 
            _fps, 10.0f, RGBColor(255.0f, 0.0f, 0.0f));

        swapBuffer(window);
        pollEvent();
    }

    terminateApplication();

    printf("------------ EXIT -------------\n");
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
                camera_dir.z += 0.02f;
                break;
            case KEY_D:
                rotate_angle += 0.05f;
                break;
            case KEY_W:
                if (camera_dir.z > 0.02f)
                {
                    camera_dir.z -= 0.02f;
                }
                break;
            case KEY_SPACE:
                printf("Press SPACE\n");
                break;
            case KEY_ESCAPE:
                exit(0);
                break;
            default:
                return;
        }
        camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);
        camera.setTransform(camera_pos, camera_tar);
        // scene.drawScene(frame_buffer, camera);
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

// void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
// {
//     printf("keyboard event : key[%d] pressed[%u]\n", key, pressed);
//     switch(key) {
//         case KEY_ESCAPE:
//             destroyWindow(window);
//             break;
//         case KEY_SPACE:
//             Time time = getSystemTime();
//             printf("year[%d] month[%d] weekday[%d] day[%d] hour[%d] minute[%d] second[%d] ms[%d]\n", 
//                 time.year, time.month, time.day_of_week, time.day, time.hour, time.minute, time.second, time.millisecond);
//             break;
//     }
// }
// void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
// {
//     __unused_variable(window);
//     printf("mouse button event : button[%d] pressed[%u]\n", button, pressed);
// }
// void mouseScrollEventCallback(AppWindow *window, float offset)
// {
//     __unused_variable(window);
//     printf("mouse scroll event : offset[%.2f]\n", offset);
// }
// void mouseDragEventCallback(AppWindow *window, float x, float y)
// {
//     __unused_variable(window);
//     printf("mouse drag event x[%.2f] y[%.2f]\n", x, y);
// }