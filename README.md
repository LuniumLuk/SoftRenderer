# Lu Renderer (Under Developement)

A Tiny Renderer designed to rely on minimum dependencies (Under Developement)

Meanwhile, this project also serve as practice while learning the book 'Real-Time Rendering'

I intend to implement a shader-based renderer that runs on CPU from scratch using c++ with minimum dependencies. It's API is similar to OpenGL, with the following features:

- Programable Shaders
- Array Buffer, Frame Buffer and Element Buffer
- Fixed Pipeline in clipping and Screen mapping
- Multi Platform

> current lib dependencies:  `stdlib.h`   `stdio.h`   `math.h`   `string.h`   `assert.h`   `utility`   `initializer_list`

<img src="images/triangle_rasterization.webp" width="400px" />

<img src="images/wireframe_bunny.gif" width="400px" />

### Content

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

- Others
  - dynamic array

### Platform Support

- MacOS

### Compile & Run

==> Build with c++11

#### MacOS

```shell
## for test run
make
## for viewer application
make mac
## compile core dll
make dll
## run program
./viewer
```

#### Windows 

(only for test, not support viewer app)

```shell
mingw32-make test
```
