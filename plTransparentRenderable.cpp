#include "plTransparentRenderable.h"

plTransparentRenderable::plTransparentRenderable()
{  
    _isTransparent = false; 
}


PLbool plTransparentRenderable::isTransparent() const 
{ 
    return _isTransparent; 
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


void plTransparentRenderable::toggleTransparency()
{
    if (_isTransparent)
    {
        setTransparent();
    }
    else
    {
        setVisible();
    }
}


void plTransparentRenderable::saveState()
{
    if ( _isTransparent )
    {
        _storedVisibilityState = PL_TRANSPARENT;
    }
    else if ( _isVisible )
    {
        _storedVisibilityState = PL_VISIBLE;
    }
    else
    {
        _storedVisibilityState = PL_NOT_VISIBLE;
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


void plTransparentRenderable::loadState()
{
    switch ( _storedVisibilityState )
    {   
        case PL_TRANSPARENT:  
        
            setTransparent();
            break;
           
        case PL_VISIBLE:  
        
            setVisible();
            break;
            
        case PL_NOT_VISIBLE:  
        
            setInvisible();
            break;       
    }
}

