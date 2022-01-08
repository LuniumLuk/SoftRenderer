#ifndef __PIPELINE_HPP__
#define __PIPELINE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"
#include "shader.hpp"
#include "buffer.hpp"

namespace Lurdr
{

void drawTriangles(
    const FrameBuffer & frame_buffer,
    const Program & program
);

}

#endif