# Lu Renderer

A Tiny Renderer relying on minimum dependencies

## Content

- basic linear algebra
  - vector
  - quaternion
  - matrix
- Image IO
  - BMP Format

## Compile and Run

### Windows:

PowerShell: `mingw32-make; .\viewer.exe`

Command Promot: `mingw32-make && .\viewer.exe `

### MacOS:

ZSH: `make; ./viewer`

macos window: `clang src/macos.m -o viewer -framework Cocoa; ./viewer`
