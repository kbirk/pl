#include "plRenderable.h"

plRenderable::plRenderable()
{  
    _isVisible = true; 
}

PLbool plRenderable::isVisible() const
{
    return _isVisible;
}

void plRenderable::toggleVisibility()
{
    _isVisible = !_isVisible;
}
