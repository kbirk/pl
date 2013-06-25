#include "plRenderable.h"

plRenderable::plRenderable()
{  
    _isVisible = true; 
}

void plRenderable::toggleVisibility()
{
    _isVisible = !_isVisible;
}

PLbool plRenderable::isVisibile() const
{
	return  _isVisible;
}
