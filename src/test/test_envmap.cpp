#include "test.hpp"

using namespace Lurdr;

static void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
static void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
static void mouseScrollEventCallback(AppWindow *window, float offset);
static void mouseDragEventCallback(AppWindow *window, float x, float y);

static AppWindow *window;
static FrameBuffer frame_buffer(512, 512);

static Scene scene;
static Entity* entity_ptr;

static float mouse_x = -1.0f; 
static float mouse_y = -1.0f;

static float camera_fov = 60.0f;
static const int SHADER_COUNT = 7;
static int current_shader = 0;

static float view_distance = 3.0f;

int test_pipeline() {

    entityConf config("assets/sphere.txt");
    Entity ent = Entity(config);
    entity_ptr = &ent;

    ent.getTriangleMesh()->computeTriangleNormals();
    ent.getTriangleMesh()->computeVertexNormals();
    ent.getTriangleMesh()->printMeshInfo();

    Envmap envmap("assets/envmaps/env01.bmp");

    scene.addEntity(&ent);
    scene.addLight((Light*)&dir_light);
    scene.addLight((Light*)&point_light1);
    scene.addLight((Light*)&point_light2);
    scene.setEnvmap(&envmap);

    vec3 mesh_center = ent.getTriangleMesh()->getMeshCenter();
    scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -view_distance), mesh_center);

    const Shader * shader = new BlinnPhongShader();

    LURDR_WIREFRAME_MODE(false);
    LURDR_BACKFACE_CULLING(true);
    LURDR_DEPTH_TEST(true);
    LURDR_TEXTURE_FILTERING(TF_LINEAR);

    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    long _fps = 0;
    FPS_SETUP();
    while (!windowShouldClose(window))
    {
        FPS_UPDATE(_fps);

        frame_buffer.clearColorBuffer(rgb(0.0f, 0.0f, 0.0f));
        Pipeline::draw(frame_buffer, scene, shader);

        drawString(
            frame_buffer, 10.0f, 10.0f,
            "FPS", 6.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 40.0f, 10.0f, 
            _fps, 6.0f, COLOR_RED);

        swapBuffer(window);
        pollEvent();
    }

    terminateApplication();
    return 0;
}

static void transformModel(float rotate_angle, float scale)
{
    vec3 mesh_center = vec3(entity_ptr->getTransform() * vec4(entity_ptr->getTriangleMesh()->getMeshCenter(), 1.0f));
    mat4 t1 = mat4::fromTRS(-mesh_center, Quaternion::IDENTITY, vec3(1.0f, 1.0f, 1.0f));
    mat4 t2 = mat4::IDENTITY.rotated(Quaternion::fromAxisAngle(vec3(0.0f, 1.0f, 0.0f), rotate_angle));
    mat4 t3 = mat4::fromTRS(mesh_center, Quaternion::IDENTITY, vec3(1.0f, 1.0f, 1.0f) * scale);
    entity_ptr->setTransform(t3 * t2 * t1 * entity_ptr->getTransform());
}

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
{
    assert(window);
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
                break;
            case KEY_SPACE:
                break;
            default:
                return;
        }
    }
}
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
{
    __unused_variable(window);
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