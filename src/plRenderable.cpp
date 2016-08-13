#include "plRenderable.h"

plRenderable::plRenderable()
    : _isVisible(true)
{
}


void plRenderable::toggleVisibility()
{
    _isVisible = !_isVisible;
}


void plRenderable::setVisible()
{
    _isVisible = true;
}


void plRenderable::setInvisible()
{
    _isVisible = false;
}


bool plRenderable::isVisible() const
{
    return _isVisible;
}
