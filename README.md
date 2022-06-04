# Soft Renderer

This is a Tiny Renderer on CPU with single-thread built from scratch, using OpenGL-like API design.

- Meanwhile, this project also serve as practice while learning the book 'Real-Time Rendering' :book: https://www.realtimerendering.com/

- This project took a lot reference from project https://github.com/zauonlok/renderer for platform deploying.

## Planned Features

I intend to implement a shader-based renderer that runs on CPU from scratch using c++ with minimum dependencies. It's APIs are similar to OpenGL, with the following features:

- Pipeline that runs on CPU (maybe GPU later, but more limited platforms)
- Programable and Extendable Shaders
- Flexible and Fast to Access Buffers
- API similar to OpenGL
- Support Cross Platform (MacOS, Windows)
- Image and Geometry Process Functionalities

### Switch Shading & Mouse Manipulation

<img src="images/phong_shading.gif" width="480px" />

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
- [ ] Wireframe Display
- [ ] Normal Mapping
- [ ] Read .mtl for Material
- [x] Basic Lighting + Phong, Blinn Shaders
- [x] Mipmapping
- [ ] Sky Box + Environment Map
- [ ] PBR
- [ ] PostProcessing Pass
- [ ] Alpha Test + Alpha Blending
- [ ] Multi-thread

## Bug Report

- Error Depth value appear in back-face

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
  - easy programable shader

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
