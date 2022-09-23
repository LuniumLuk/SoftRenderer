#include "entity.hpp"

using namespace LuGL;

EntityConfig::EntityConfig(const char * filename):
    mesh_filename(nullptr),
    albedo_map(nullptr),
    diffuse_map(nullptr),
    specular_map(nullptr),
    normal_map(nullptr),
    scale(vec3(1.0f, 1.0f, 1.0f))
{
    loadFromFile(filename);
}

void EntityConfig::loadFromFile(const char * filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == nullptr)
    {
        printf("EntityConfig : config file: %s open failed\n", filename);
        return;
    }

    this->~EntityConfig();

    char line_buffer[MAX_CONF_LINE];
    char filename_buffer[MAX_CONF_LINE];
    int scanned_items;

    while (fgets(line_buffer, MAX_CONF_LINE, fp))
    {
        if (strncmp(line_buffer, "mesh ", 5) == 0)
        {
            scanned_items = sscanf(line_buffer, "mesh %s", filename_buffer);
            if (scanned_items == 1)
            {
                mesh_filename = new char[strlen(filename_buffer) + 1];
                strcpy(mesh_filename, filename_buffer);
            }
        }
        else if (strncmp(line_buffer, "albedo ", 7) == 0)
        {
            scanned_items = sscanf(line_buffer, "albedo %s", filename_buffer);
            if (scanned_items == 1)
            {
                albedo_map = new char[strlen(filename_buffer) + 1];
                strcpy(albedo_map, filename_buffer);
            }
        }
        else if (strncmp(line_buffer, "diffuse ", 8) == 0)
        {
            scanned_items = sscanf(line_buffer, "diffuse %s", filename_buffer);
            if (scanned_items == 1)
            {
                diffuse_map = new char[strlen(filename_buffer) + 1];
                strcpy(diffuse_map, filename_buffer);
            }
        }
        else if (strncmp(line_buffer, "specular ", 9) == 0)
        {
            scanned_items = sscanf(line_buffer, "specular %s", filename_buffer);
            if (scanned_items == 1)
            {
                specular_map = new char[strlen(filename_buffer) + 1];
                strcpy(specular_map, filename_buffer);
            }
        }
        else if (strncmp(line_buffer, "normal ", 7) == 0)
        {
            scanned_items = sscanf(line_buffer, "normal %s", filename_buffer);
            if (scanned_items == 1)
            {
                normal_map = new char[strlen(filename_buffer) + 1];
                strcpy(normal_map, filename_buffer);
            }
        }
        else if (strncmp(line_buffer, "scale ", 6) == 0)
        {
            float x, y, z;
            scanned_items = sscanf(line_buffer, "scale %f %f %f", &x, &y, &z);
            if (scanned_items == 3)
            {
                scale.x = x;
                scale.y = y;
                scale.z = z;
            }
        }
    }
}

EntityConfig::~EntityConfig()
{
    if (mesh_filename)  delete mesh_filename;
    if (albedo_map)     delete albedo_map;
    if (diffuse_map)    delete diffuse_map;
    if (specular_map)   delete specular_map;
    if (normal_map)     delete normal_map;
    mesh_filename = nullptr;
    albedo_map = nullptr;
    diffuse_map = nullptr;
    specular_map = nullptr;
    normal_map = nullptr;
}

Entity::Entity():
    m_transform(mat4::IDENTITY),
    m_distance(0.0f),
    m_material(nullptr),
    m_mesh(nullptr),
    m_material_need_delete(false),
    m_mesh_need_delete(false) {}

Entity::Entity(const entityConf & config):
    Entity()
{
    if (config.mesh_filename)
    {
        m_mesh = new TriangleMesh(config.mesh_filename);
        m_mesh_need_delete = true;
    }

    m_material = new Material();
    m_material_need_delete = true;

    if (config.albedo_map)
    {
        m_material->albedo.loadTextureSurface(config.albedo_map);
    }
    if (config.diffuse_map)
    {
        m_material->diffuse.loadTextureSurface(config.diffuse_map);
    }
    if (config.specular_map)
    {
        m_material->specular.loadTextureSurface(config.specular_map);
    }
    if (config.normal_map)
    {
        m_material->normal.loadTextureSurface(config.normal_map);
    }
    vec3 center = m_mesh->getMeshCenter();
    m_transform = mat4::fromTRS(-center, Quaternion::IDENTITY, vec3(1.0f, 1.0f, 1.0f));
    m_transform.scale(config.scale);
}

Entity::~Entity()
{
    if (m_material_need_delete) delete m_material;
    if (m_mesh_need_delete) delete m_mesh;
}

bool Entity::compareDistance(Entity * const & a, Entity * const & b)
{
    return a->m_distance < b->m_distance;
}
