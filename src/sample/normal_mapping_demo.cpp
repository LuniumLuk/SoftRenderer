#include "sample.hpp"

using namespace LuGL;

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

static float view_distance = 3.0f;

static vec3 mesh_center;
static float light_angle = 0.0f;

static DirectionalLight dir_light(
    vec3(0.0f, 0.0f, 0.0f),
    vec3(cos(light_angle), -1.0f, sin(light_angle)),
    vec3(1.0f, 1.0f, 1.0f) * 1.0f,
    vec3(1.0f, 1.0f, 1.0f) 
);

static int current_shader = 0;
static const int SHADER_NUM = 4;

class LightingShader : public LitShader
{
public:
    virtual vec4 frag(const v2f in, const Entity * entity, const Scene & scene) const
    {
        const float diffuse_strength = 1.0f;
        const float specular_strength = 0.8f;

        vec3 normal = vec3(SAMPLER_2D(TEXTURE_NORMAL, in.texcoord)) * 2.0f - vec3(1.0f, 1.0f, 1.0f);
        normal = (TBN_MATRIX * normal).normalized();

        vec3 view_dir = (scene.getCamera().getPosition() - in.frag_pos).normalized();
        LightComp light_comp = scene.calcLight(normal, in.frag_pos, view_dir);
        vec3 color = diffuse_strength * light_comp.diffuse + specular_strength * light_comp.specular;

        return vec4(color, 1.0f);
    }
};

int normal_mapping_demo() {

    entityConf config("assets/teapot_low.txt");
    Entity ent = Entity(config);
    entity_ptr = &ent;

    ent.getTriangleMesh()->computeTriangleNormals();
    ent.getTriangleMesh()->computeVertexNormals();
    ent.getTriangleMesh()->computeTangentVectors();
    ent.getTriangleMesh()->printMeshInfo();
    mesh_center = ent.getTriangleMesh()->getMeshCenter();
    // ent.setTransform(mat4::fromAxisAngle(vec3::UNIT_X, -PI / 2));

    Envmap envmap("assets/envmaps/env01.bmp");

    scene.addEntity(&ent);
    scene.addLight((Light*)&dir_light);
    scene.setEnvmap(&envmap);

    Shader* shaders[SHADER_NUM] = {
        (Shader*) new VertexNormalShader(),
        (Shader*) new NormalMappingShader(),
        (Shader*) new LightingShader(),
        (Shader*) new BlinnPhongShader(),
    };

    vec3 mesh_center = ent.getTriangleMesh()->getMeshCenter();
    scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -view_distance), mesh_center);

    LUGL_WIREFRAME_MODE(false);
    LUGL_BACKFACE_CULLING(true);
    LUGL_DEPTH_TEST(true);
    LUGL_TEXTURE_FILTERING(TF_LINEAR);
    LUGL_SAMPLE_OPTION(LUGL_SAMPLE_DEFAULT);
    frame_buffer.setupSamplingOption();

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

        scene.getLights()->at(0)->setDirection(vec3(cos(light_angle), -1.0f, sin(light_angle)));
        Pipeline::draw(frame_buffer, scene, shaders[current_shader]);

        drawString(
            frame_buffer, 10.0f, 5.0f,
            "FPS", 4.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 40.0f, 5.0f, 
            _fps, 4.0f, COLOR_RED);

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
                light_angle += 0.1f;
                break;
            case KEY_S:
                break;
            case KEY_D:
                light_angle -= 0.1f;
                break;
            case KEY_W:
                break;
            case KEY_ESCAPE:
                destroyWindow(window);
                break;
            case KEY_SPACE:
                // reset camera
                camera_fov = 60.0f;
                scene.getCamera().setFOV(camera_fov / 180.0f * PI);
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -view_distance), mesh_center);
                scene.getCamera().setUp(vec3::UNIT_Y);
                break;
            case KEY_I:
                current_shader = (current_shader + 1) % SHADER_NUM;
                break;
            case KEY_O:
                break;
            case KEY_P: // save screenshot
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