#include "plRenderable.h"

plRenderable::plRenderable()
{  
    isVisible = true; 
}

void plRenderable::toggleVisibility()
{
    isVisible = !isVisible;
}
