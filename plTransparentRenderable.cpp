#include "plTransparentRenderable.h"

plTransparentRenderable::plTransparentRenderable()
{  
    _isTransparent = false; 
}


void plTransparentRenderable::toggleVisibility()
{
    if (_isTransparent) 
    {
        setInvisible();
    } 
    else if (!_isVisible) 
    {
        setVisible();
    } 
    else 
    {
        setTransparent();
    }
}


void plTransparentRenderable::setVisible()
{
    _isVisible = true;
    _isTransparent = false;
}


void plTransparentRenderable::setTransparent()
{
    _isVisible = true;
    _isTransparent = true;
}


void plTransparentRenderable::setInvisible()
{
    _isVisible = false;
    _isTransparent = false;
}



PLbool plTransparentRenderable::isTransparent() const 
{ 
    return _isTransparent; 
}


