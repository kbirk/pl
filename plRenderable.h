#ifndef __PL_RENDERABLE_H_
#define __PL_RENDERABLE_H_

#include "plCommon.h"
#include "plVector3.h"

enum visibilityState
{
    PL_NOT_VISIBLE = 0,
    PL_VISIBLE,
    PL_TRANSPARENT,
};


class plRenderable
{
    
    public:
    
        plRenderable();  
        
        virtual void   toggleVisibility();		
		virtual PLbool isVisible() const;
		virtual void   saveState();
		virtual void   loadState();
		//virtual void   draw() const;
		
    protected:     
           
        PLbool _isVisible;
        PLuint _storedVisibilityState;
    
};


#endif 
