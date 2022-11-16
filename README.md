# Soft Renderer

This is a Tiny Renderer on CPU built from scratch, using OpenGL-similar API design. this project served as learning project and took quite a lot reference from project https://github.com/zauonlok/renderer for multi-platform depolying.

## Features

I intend to implement a shader-based renderer that runs on CPU from scratch using c++ with minimum dependencies, with the following features:

- CPU rasterization
- Shading via shaders
- Similar to OpenGL API
- Cross Platform (MacOS, Windows)
- Image and Geometry Processing

### Shader-based pipeline

A simple example for creating a shader, simply inherit class Shader, you are able to access lighting, scene, mvp matrix, interpolated attributes and so on.

```c++
#include "src/api.hpp"
using namespace LuGL;

class ExampleShader : public Shader
{
public:
    virtual v2f vert(const vdata in, const Entity * entity, const Scene & scene) const;
    virtual vec4 frag(const v2f in, const Entity * entity, const Scene & scene) const;
};

v2f ExampleShader::vert(const vdata in, const Entity * entity, const Scene & scene) const
{
    v2f out;

    out.position = MVP_MATRIX * vec4(in.position, 1.0f);
    out.texcoord = in.texcoord;
    out.normal   = MODEL_INV_TRANSPOSE * in.normal;

    return out;
}

vec4 ExampleShader::frag(const v2f in, const Entity * entity, const Scene & scene) const
{
    rgb color = rgb(
        in.normal.x * 0.5f + 0.5f,
        in.normal.y * 0.5f + 0.5f,
        in.normal.z * 0.5f + 0.5f );
    
    return vec4(color.normalized(), 1.0f);
}
```

### AntiAliasing

8X MSAA    |  4X MSAA | 2X MSAA | No AA
:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:
<img src="images/8xMSAA.jpg" width="200px" />  |  <img src="images/4xMSAA.jpg" width="200px" /> | <img src="images/2xMSAA.jpg" width="200px" /> | <img src="images/NOAA.jpg" width="200px" />
7 FPS | 10 FPS | 12 FPS | 21 FPS

### Switch Shading & Mouse Manipulation

> Code Sample in `src/sample/demo.cpp`

Teapot    |  Spot
:-------------------------:|:-------------------------:
<img src="images/phong_shading.gif" width="360px" />  |  <img src="images/spot_model.gif" width="360px" />

Triangle Normal Shading    |  Vertex Normal Shading
:-------------------------:|:-------------------------:
<img src="images/triangle_normal_shading_2.png" width="360px" />  |  <img src="images/vertex_normal_shading_2.png" width="360px" />
<img src="images/triangle_normal_shading.png" width="360px" />  |  <img src="images/vertex_normal_shading.png" width="360px" />

## Todos

- [x] Basic Math Liberary
- [x] Basic Resource IO (.obj, .bmp)
- [x] Basic Rasterization Implementation
- [x] Basic Graphics Pipeline that runs on single thread
- [x] Win32 Application API
- [x] Simple Shaders (Unlit, VertexNormal, TriangleNormal)
- [x] Hovering Camera by Mouse Drag
- [x] Implement Barycentric Triangle Rastrization with Perspective Correction Interpolation
- [x] Wireframe Display
- [x] Multisample anti-aliasing (2X/4X/8X)
- [x] Normal Mapping
- [ ] Read .mtl for Material
- [x] Basic Lighting + Phong, Blinn Shaders
- [ ] Mipmapping
- [ ] Sky Box + Environment Map
- [ ] PBR
- [ ] PostProcessing Pass
- [ ] Shadow
- [ ] SSAO
- [ ] SS Reflection
- [ ] Alpha Test + Alpha Blending
- [x] Multi-threading using openmp

## Bug Report

- MacOS screenshot produce error BMP file

## Current Features

- basic linear algebra
  - vector
  - quaternion
  - matrix

- IO
  - BMP Format
  - OBJ Format

- Graphics
  - basic rasterization algorithm
  - colormap
  - digit display
  - programable shader
  - MSAA

- Others
  - dynamic array
  - quick sort

## Current Platform

MacOS (Cocoa App)

Windows (Win32 App)

## Compile & Run

### MacOS

- compile

```zsh
make macos
```

- run

```zsh
./viewer
```

### Windows 

- compile

```shell
mingw32-make win32
```

- run

```shell
viewer
```
