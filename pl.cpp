#include "pl.h"

void plInit()
{   
    // initialize openGL specific buffers/shaders
    plOpenGLInfo::init();
    plRenderer::init();
    plPicking::init();
}



