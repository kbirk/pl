#include "pl.h"

void plInit()
{   
    // initialize openGL specific buffers/shaders
    plRenderer::init();
    plPicking::init();
}



