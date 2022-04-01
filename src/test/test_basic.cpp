#include "test.hpp"

using namespace Lurdr;

int test_basic()
{
    entityConf config("assets/config01.txt");

    Entity ent = Entity(config);

    ent.getTriangleMesh()->printMeshInfo();
    
    Texture::sampler(ent.getMaterial()->albedo, vec2(0.123f, 0.122f)).print();
    Texture::sampler(ent.getMaterial()->specular, vec2(0.982f, 0.129f)).print();

    Scene scene;
    FrameBuffer frame_buffer(512, 512);

    scene.addEntity(&ent);

    vec3 mesh_center = ent.getTriangleMesh()->getMeshCenter();
    mesh_center.print();
    scene.getCamera().setTransform(mesh_center + vec3(0.0f, 0.0f, -2.0f), mesh_center);

    UnlitShader shader;
    
    {
        frame_buffer.clearColorBuffer(rgb(0.2f, 0.5f, 1.0f));
        Timer t("draw");
        Pipeline::draw(frame_buffer, scene, (Shader*)&shader);
    }

    return 0;
}