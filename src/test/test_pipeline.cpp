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
static const int SHADER_COUNT = 3;
static int current_shader = 0;

int test_pipeline() {

    FrameBuffer frame_buffer(512, 512);

    entityConf config("assets/config04.txt");
    Entity ent = Entity(config);
    ent.getTriangleMesh()->computeTriangleNormals();
    ent.getTriangleMesh()->computeVertexNormals();
    ent.getTriangleMesh()->printMeshInfo();

    scene.addEntity(&ent);

    mesh_center = ent.getTriangleMesh()->getMeshCenter();
    scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -4.0f), mesh_center);

    Shader* shaders[SHADER_COUNT] = {
        (Shader*)new UnlitShader(),
        (Shader*)new TriangleNormalShader(),
        (Shader*)new VertexNormalShader()
    };

    char shader_names[SHADER_COUNT][64] = {
        "UNLIT",
        "TRIANGLE NORMAL",
        "VERTEX NORMAL",
    };

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

        frame_buffer.clearColorBuffer(rgb(0.0f, 0.0f, 0.0f));
        Pipeline::draw(frame_buffer, scene, shaders[current_shader]);

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
            "MOUSE SCROLL --------- CAMERA FOV", 6.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 350.0f, 75.0f, 
            (int)camera_fov, 6.0f, COLOR_RED);
        drawString(
            frame_buffer, 10.0f, 90.0f,
            "KEY SPACE    --------- SWITCH SHADER", 6.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 350.0f, 90.0f,
            shader_names[current_shader], 6.0f, COLOR_RED);
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
                // switch shader
                current_shader++;
                if (current_shader >= SHADER_COUNT)
                {
                    current_shader = 0;
                }
#if 0
                // reset camera
                model_rotation_angle = 0.0f;
                model_scale = 1.0f;
                camera_fov = 60.0f;
                scene.getCamera().setFOV(camera_fov / 180.0f * PI);
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -4.0f), mesh_center);
                scene.getCamera().setUp(vec3::UNIT_Y);
#endif
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