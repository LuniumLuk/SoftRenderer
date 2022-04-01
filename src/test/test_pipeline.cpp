#include "test.hpp"

using namespace Lurdr;

static void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
static void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
static void mouseScrollEventCallback(AppWindow *window, float offset);
static void mouseDragEventCallback(AppWindow *window, float x, float y);

static AppWindow *window;
static FrameBuffer frame_buffer(512, 512);

static Scene scene;
static vec3 mesh_center;
static float model_rotation_angle = 0.0f;
static float camera_to_model_dist = 10.0f;

int test_pipeline() {

    FrameBuffer frame_buffer(512, 512);

    entityConf config("assets/config01.txt");
    Entity ent = Entity(config);
    ent.getTriangleMesh()->computeTriangleNormals();
    ent.getTriangleMesh()->printMeshInfo();

    scene.addEntity(&ent);

    mesh_center = ent.getTriangleMesh()->getMeshCenter();
    scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, camera_to_model_dist), mesh_center);

    // UnlitShader shader;
    // TriangleNormalShader shader;
    VertexNormalShader shader;

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
        mat4 t3 = mat4::fromTRS(mesh_center, Quaternion::IDENTITY, vec3(1.0f, 1.0f, 1.0f));
        ent.setTransform(t3 * t2 * t1);
        // ent.setTransform(t2);

        frame_buffer.clearColorBuffer(rgb(0.0f, 0.2f, 0.5f));
        Pipeline::draw(frame_buffer, scene, (Shader*)&shader);

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
                camera_to_model_dist += 0.2f;
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, camera_to_model_dist), mesh_center);
                break;
            case KEY_D:
                model_rotation_angle += 0.1f;
                break;
            case KEY_W:
                camera_to_model_dist -= 0.2f;
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, camera_to_model_dist), mesh_center);
                break;
            case KEY_ESCAPE:
                destroyWindow(window);
                break;
            case KEY_SPACE:
                model_rotation_angle = 0.0f;
                camera_to_model_dist = -10.0f;
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, camera_to_model_dist), mesh_center);
                break;
            default:
                return;
        }
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