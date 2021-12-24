# Lu Renderer

A Tiny Renderer designed to rely on minimum dependencies

Meanwhile, this project also serve as practice while learning the book 'Real-Time Rendering'

I intend to implement a shader-based rendered from scratch using c++ and its basic libraries only

> dependencies:  `stdlib.h`   `stdio.h`   `math.h`   `string.h`   `assert.h`   `utility`

<img src="images/triangle_rasterization.webp" width="400px" />

<img src="images/wireframe_bunny.webp" width="400px" />

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

#### MacOS

```shell
## for test run
make
## for viewer application
make mac
## run program
./viewer
```

#### Windows

```shell
mingw32-make
```

then click **viewer.exe** and run
