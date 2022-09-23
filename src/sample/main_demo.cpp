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
static const int SHADER_COUNT = 8;
static int current_shader = 0;

static bool blue_light_rotate = false;
static bool red_light_rotate = false;
static float blue_light_angle = PI;
static float red_light_angle = 0.0f;

static float view_distance = 3.0f;

static DirectionalLight dir_light(
    vec3(0.0f, 0.0f, 0.0f),
    vec3(1.0f, -1.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f) * 1.0f,
    vec3(1.0f, 1.0f, 1.0f) 
);

static PointLight point_light1(
    vec3(2.0 * cosf(red_light_angle), -2.0f, 2.0 * sinf(red_light_angle)),
    vec3(1.0f, 1.0f, 1.0f).normalized(),
    vec3(1.0f, 0.0f, 0.5f) * 1.0f,
    vec3(1.0f, 0.0f, 0.5f)
);

static PointLight point_light2(
    vec3(3.0 * cosf(blue_light_angle), 2.0f, 3.0 * sinf(blue_light_angle)),
    vec3(2.0f, 0.0f, -4.0f).normalized(),
    vec3(0.0f, 0.5f, 1.0f) * 1.0f,
    vec3(0.0f, 0.5f, 1.0f)
);

const vec4 base_color = vec4(
    188.0f / 255.0f, 
    159.0f / 255.0f, 
    119.0f / 255.0f, 
    1.0f
);

class LightShader : public LitShader
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

int main_demo(const char* model)
{
    char model_filename[256] = { 0 };
    sprintf(model_filename, "assets/%s.txt", model);
    entityConf config(model_filename);
    Entity ent = Entity(config);
    entity_ptr = &ent;
    // ent.getMaterial()->albedo.setBaseColor(base_color);
    // ent.getMaterial()->diffuse.setBaseColor(base_color);
    // ent.getMaterial()->specular.setBaseColor(base_color);

    ent.getTriangleMesh()->computeTriangleNormals();
    ent.getTriangleMesh()->computeVertexNormals();
    ent.getTriangleMesh()->computeTangentVectors();
    ent.getTriangleMesh()->printMeshInfo();
    // ent.setTransform(mat4::fromAxisAngle(vec3::UNIT_X, -PI / 2));

    // Envmap envmap("assets/envmaps/env01.bmp");

    scene.addEntity(&ent);
    scene.addLight((Light*)&dir_light);
    scene.addLight((Light*)&point_light1);
    scene.addLight((Light*)&point_light2);
    // scene.setEnvmap(&envmap);

    // vec3 mesh_center = ent.getTriangleMesh()->getMeshCenter();
    // scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -view_distance), mesh_center);
    scene.getCamera().setTransform(vec3(0.0f, 0.0f, -view_distance), vec3(0.0f, 0.0f, 0.0f));


    Shader* shaders[SHADER_COUNT] = {
        (Shader*)new UnlitShader(),
        (Shader*)new UnlitShader(),
        (Shader*)new LightShader(),
        (Shader*)new BlinnPhongShader(),
        (Shader*)new TriangleNormalShader(),
        (Shader*)new VertexNormalShader(),
        (Shader*)new NormalMappingShader(),
        (Shader*)new DepthShader()
    };

    char shader_names[SHADER_COUNT][64] = {
        "UNLIT",
        "WIREFRAME",
        "BLINN-PHONG",
        "LIGHT",
        "TRIANGLE NORMAL",
        "VERTEX NORMAL",
        "NORMAL MAPPING",
        "DEPTH"
    };
    char sample_option_names[LUGL_SAMPLE_OPTION_NUM][64] = {
        "DEFAULT",
        "2X MSAA",
        "4X MSAA",
        "8X MSAA",
    };

    LUGL_WIREFRAME_MODE(false);
    LUGL_BACKFACE_CULLING(true);
    LUGL_DEPTH_TEST(true);
    LUGL_TEXTURE_FILTERING(TF_LINEAR);
    LUGL_SAMPLE_OPTION(LUGL_SAMPLE_4xMSAA);
    frame_buffer.setupSamplingOption();

    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    long _fps = 0;
    float _delta = 0;
    FPS_SETUP();
    while (!windowShouldClose(window))
    {
        FPS_UPDATE(_fps, _delta);

        if (red_light_rotate)
        {
            red_light_angle += 1.0f * _delta;
            scene.getLights()->at(1)->setPosition(vec3(2.0 * cosf(red_light_angle), -2.0f, 2.0 * sinf(red_light_angle)));
        }
        if (blue_light_rotate)
        {
            blue_light_angle += 2.0f * _delta;
            scene.getLights()->at(2)->setPosition(vec3(3.0 * cosf(blue_light_angle), 3.0f, 3.0 * sinf(blue_light_angle)));
        }

        frame_buffer.clearColorBuffer(rgb(0.0f, 0.0f, 0.0f));
        Pipeline::draw(frame_buffer, scene, shaders[current_shader]);

        drawString(
            frame_buffer, 10.0f, 5.0f,
            "FPS", 4.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 40.0f, 5.0f, 
            _fps, 4.0f, COLOR_RED);
#if 1
        drawString(
            frame_buffer, 10.0f, 20.0f,
            "KEY A D      --------- ROTATE MODEL", 4.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 30.0f,
            "KEY S W      --------- TOGGLE LIGHT ROTATE", 4.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 40.0f,
            "MOUSE DRAG   --------- ROTATE CAMERA", 4.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 50.0f,
            "MOUSE SCROLL --------- CAMERA FOV", 4.0f, COLOR_WHITE);
        drawInteger(
            frame_buffer, 240.0f, 50.0f, 
            (int)camera_fov, 4.0f, COLOR_RED);
        drawString(
            frame_buffer, 10.0f, 60.0f,
            "KEY SPACE    --------- SWITCH SHADER", 4.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 240.0f, 60.0f,
            shader_names[current_shader], 4.0f, COLOR_RED);
        drawString(
            frame_buffer, 10.0f, 70.0f,
            "KEY ESCAPE   --------- EXIT", 4.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 10.0f, 80.0f,
            "KEY O        --------- TOGGLE MSAA", 4.0f, COLOR_WHITE);
        drawString(
            frame_buffer, 240.0f, 80.0f,
            sample_option_names[Singleton<Global>::get().sample_option], 4.0f, COLOR_RED);
        drawString(
            frame_buffer, 10.0f, 90.0f,
            "KEY P        --------- SCREEN SHOT", 4.0f, COLOR_WHITE);
#endif
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
                transformModel(-0.1f, 1.0f);
                break;
            case KEY_S:
                // {
                //     vec3 pos = point_light1.getPosition();
                //     pos.y -= 0.4f;
                //     if (pos.y < -4.0f) pos.y = -4.0f;
                //     point_light1.setPosition(pos);
                // }
                blue_light_rotate = !blue_light_rotate;
                // transformModel(0.0f, 0.9f);
                break;
            case KEY_D:
                transformModel(0.1f, 1.0f);
                break;
            case KEY_W:
                // {
                //     vec3 pos = point_light1.getPosition();
                //     pos.y += 0.4f;
                //     if (pos.y > 4.0f) pos.y = 4.0f;
                //     point_light1.setPosition(pos);
                // }
                red_light_rotate = !red_light_rotate;
                // transformModel(0.0f, 1.1f);
                break;
            case KEY_ESCAPE:
                destroyWindow(window);
                break;
            case KEY_SPACE:
#if 1
                // switch shader
                current_shader++;
                if (current_shader >= SHADER_COUNT)
                {
                    current_shader = 0;
                }
                if (current_shader == 1)
                {
                    LUGL_WIREFRAME_MODE(true);
                }
                else
                {
                    LUGL_WIREFRAME_MODE(false);
                }
#endif
#if 0
                // reset camera
                model_rotation_angle = 0.0f;
                model_scale = 1.0f;
                camera_fov = 60.0f;
                scene.getCamera().setFOV(camera_fov / 180.0f * PI);
                scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -view_distance), mesh_center);
                scene.getCamera().setUp(vec3::UNIT_Y);
#endif
                break;
            case KEY_I:
                break;
            case KEY_O:
                LUGL_SAMPLE_OPTION((Singleton<Global>::get().sample_option + 1) % LUGL_SAMPLE_OPTION_NUM);
                frame_buffer.setupSamplingOption();
                break;
            case KEY_P: // save screenshot
                {
                    size_t img_size = frame_buffer.getWidth() * frame_buffer.getHeight() * 3;
                    UniformImage ss_uni_img(frame_buffer.getWidth(), frame_buffer.getHeight());
                    memcpy(ss_uni_img.getImageBuffer(), frame_buffer.colorBuffer(), img_size);
                    ss_uni_img.convertColorSpace(COLOR_BGR);

                    BMPImage screenshot_img(frame_buffer.getWidth(), frame_buffer.getHeight());
                    memcpy(screenshot_img.getImageBuffer(), ss_uni_img.getImageBufferConst(), img_size);
                    screenshot_img.setReverseY(true);
                    screenshot_img.writeImage("screenshot.bmp");
                }
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