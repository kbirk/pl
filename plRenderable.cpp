#include "plRenderable.h"

plRenderable::plRenderable()
{  
    _isVisible = true; 
    _storedVisibilityState = true;
}


PLbool plRenderable::isVisible() const
{
    return _isVisible;
}


void plRenderable::toggleVisibility()
{
    _isVisible = !_isVisible;
}


void plRenderable::saveState()
{
    if ( _isVisible )
    {
        _storedVisibilityState = PL_VISIBLE;
    }
    else
    {
        _storedVisibilityState = PL_NOT_VISIBLE;
    }
}


void plRenderable::loadState()
{
    switch ( _storedVisibilityState )
    {   
        case PL_VISIBLE:      _isVisible = true;    break;            
        case PL_NOT_VISIBLE:  _isVisible = false;   break;       
    }
}

