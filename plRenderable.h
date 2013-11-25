#ifndef PL_RENDERABLE_H
#define PL_RENDERABLE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderComponent.h"

enum plVisibilityStateEnum
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
		virtual void   setVisible();
		virtual void   setInvisible();
		virtual void   saveState();
		virtual void   loadState();
		
        virtual void   extractRenderComponents( plRenderMap& renderMap ) const = 0;
        virtual void   extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const = 0;
        
    protected:     
           
        PLbool _isVisible;
        PLuint _storedVisibilityState;
    
};


#endif 
