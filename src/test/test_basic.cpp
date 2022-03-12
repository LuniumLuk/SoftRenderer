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

    UnlitShader shader;
    
    {
        Timer t("draw");
        Pipeline::draw(frame_buffer, scene, (Shader*)&shader);
    }

    return 0;
}