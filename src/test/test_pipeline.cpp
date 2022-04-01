#include "test.hpp"

using namespace Lurdr;

static void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
static void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
static void mouseScrollEventCallback(AppWindow *window, float offset);
static void mouseDragEventCallback(AppWindow *window, float x, float y);

static AppWindow *window;
static FrameBuffer frame_buffer(512, 512);

static Scene scene;
static float model_rotation_angle = 0.0f;
static float model_scale = 1.0f;
static vec3 mesh_center;

static float mouse_x = -1.0f; 
static float mouse_y = -1.0f;

static float camera_fov = 60.0f;


int test_pipeline() {

    FrameBuffer frame_buffer(512, 512);

    entityConf config("assets/config01.txt");
    Entity ent = Entity(config);
    ent.getTriangleMesh()->computeTriangleNormals();
    ent.getTriangleMesh()->printMeshInfo();

    scene.addEntity(&ent);

    mesh_center = ent.getTriangleMesh()->getMeshCenter();
    scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -10.0f), mesh_center);

    // UnlitShader shader;
    TriangleNormalShader shader;
    // VertexNormalShader shader;

    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    long _fps = 0;
    long _frame_count_since_last_update = 0;
    clock_t _last_update = clock();
    while (!windowShouldClose(window))
    {
        _frame_count_since_last_update++;
        
        if (clock() - _last_update > CLOCKS_PER_SEC)
        {
            _last_update = clock();
            _fps = _frame_count_since_last_update;
            _frame_count_since_last_update = 0;
        }

        mat4 t1 = mat4::fromTRS(-mesh_center, Quaternion::IDENTITY, vec3(1.0f, 1.0f, 1.0f));
        mat4 t2 = mat4::IDENTITY.rotated(Quaternion::fromAxisAngle(vec3(0.0f, 1.0f, 0.0f), model_rotation_angle));
        mat4 t3 = mat4::fromTRS(mesh_center, Quaternion::IDENTITY, vec3(1.0f, 1.0f, 1.0f) * model_scale);
        ent.setTransform(t3 * t2 * t1);
        // ent.setTransform(t2);

        frame_buffer.clearColorBuffer(rgb(0.0f, 0.2f, 0.5f));
        Pipeline::draw(frame_buffer, scene, (Shader*)&shader);

        drawString(
            frame_buffer, 10.0f, 10.0f,
            "FPS", 6.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 40.0f, 10.0f, 
            _fps, 6.0f, COLOR_RED);
        drawString(
            frame_buffer, 10.0f, 30.0f,
            "KEY A D      --------- ROTATE MODEL", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 45.0f,
            "KEY S W      --------- SCALE MODEL", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 60.0f,
            "MOUSE DRAG   --------- ROTATE CAMERA", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 75.0f,
            "MOUSE SCROLL --------- CHANGE CAMERA FOV", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 90.0f,
            "KEY SPACE    --------- RESET ALL", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 105.0f,
            "KEY ESCAPE   --------- EXIT", 6.0f, COLOR_WHITE);
        

        swapBuffer(window);
        pollEvent();
    }

    terminateApplication();
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
                model_rotation_angle -= 0.1f;
                break;
            case KEY_S:
                model_scale *= 0.9f;
                break;
            case KEY_D:
                model_rotation_angle += 0.1f;
                break;
            case KEY_W:
                model_scale *= 1.1f;
                break;
            case KEY_ESCAPE:
                destroyWindow(window);
                break;
            case KEY_SPACE:
                model_rotation_angle = 0.0f;
                model_scale = 1.0f;
                // reset camera
                camera_fov = 60.0f;
                scene.getCamera().setFOV(camera_fov / 180.0f * PI);
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -10.0f), mesh_center);
                scene.getCamera().setUp(vec3::UNIT_Y);
                break;
            default:
                return;
        }
    }
}
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
{
    __unused_variable(window);
    // when left button is unclicked
    if (button == BUTTON_L && !pressed)
    {
        mouse_x = -1.0f;
        mouse_y = -1.0f;
    }
}
void mouseScrollEventCallback(AppWindow *window, float offset)
{
    __unused_variable(window);
    camera_fov += offset;
    if (camera_fov > 90.0f)
    {
        camera_fov = 90.0f;
    }
    else if (camera_fov < 10.0f)
    {
        camera_fov = 10.0f;
    }
    scene.getCamera().setFOV(camera_fov / 180.0f * PI);
}
void mouseDragEventCallback(AppWindow *window, float x, float y)
{
    __unused_variable(window);
    
    if (mouse_x > 0.0f && mouse_y > 0.0f)
    {
        scene.getCamera().rotateByDrag(x - mouse_x, y - mouse_y, 0.015f);
    }
    mouse_x = x;
    mouse_y = y;
}