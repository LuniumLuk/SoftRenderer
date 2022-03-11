#include "entity.hpp"

using namespace Lurdr;

EntityConfig::EntityConfig(const char * filename):
    mesh_filename(nullptr),
    ambient_map(nullptr),
    diffuse_map(nullptr),
    specular_map(nullptr),
    normal_map(nullptr)
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
        else if (strncmp(line_buffer, "ambient ", 8) == 0)
        {
            scanned_items = sscanf(line_buffer, "ambient %s", filename_buffer);
            if (scanned_items == 1)
            {
                ambient_map = new char[strlen(filename_buffer) + 1];
                strcpy(ambient_map, filename_buffer);
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
    }
}

EntityConfig::~EntityConfig()
{
    if (mesh_filename)  delete mesh_filename;
    if (ambient_map)    delete ambient_map;
    if (diffuse_map)    delete diffuse_map;
    if (specular_map)   delete specular_map;
    if (normal_map)     delete normal_map;
    mesh_filename = nullptr;
    ambient_map = nullptr;
    diffuse_map = nullptr;
    specular_map = nullptr;
    normal_map = nullptr;
}

Entity::Entity():
    m_transform(mat4::IDENTITY),
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

    if (config.ambient_map || config.diffuse_map || config.specular_map || config.normal_map)
    {
        m_material = new Material();
        m_material_need_delete = true;

        if (config.ambient_map)
        {
            m_material->ambient.loadTextureSurface(config.ambient_map);
        }
        if (config.diffuse_map)
        {
            m_material->diffuse.loadTextureSurface(config.ambient_map);
        }
        if (config.specular_map)
        {
            m_material->specular.loadTextureSurface(config.ambient_map);
        }
        if (config.normal_map)
        {
            m_material->normal.loadTextureSurface(config.ambient_map);
        }
    }
}

Entity::~Entity()
{
    if (m_material_need_delete) delete m_material;
    if (m_mesh_need_delete) delete m_mesh;
}

