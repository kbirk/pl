#include "plRenderable.h"

plRenderable::plRenderable()
{  
    _isVisible = true; 
    _storedVisibilityState = PL_VISIBLE;
}


PLbool plRenderable::isVisible() const
{
    return _isVisible;
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
        case PL_VISIBLE:  
            
            setVisible();   
            break;    
                    
        case PL_NOT_VISIBLE: 
         
            setInvisible();  
            break;       
    }
}

