#include "pl.h"

void plInit()
{
    // initialize openGL specific objects
    plOpenGLInfo::init();
    plRenderer::init();
    plRenderResources::init();
}
