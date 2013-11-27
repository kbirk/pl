#ifndef PL_TRANSPARENT_RENDERABLE_H
#define PL_TRANSPARENT_RENDERABLE_H

#include "plCommon.h"
#include "plRenderable.h"


class plTransparentRenderable : public plRenderable
{
    
    public:
    
        plTransparentRenderable();  
               
        virtual void toggleVisibility();	
        virtual void setVisible();
		virtual void setTransparent();
		virtual void setInvisible();
		
		PLbool isTransparent() const;

        virtual void extractRenderComponents( plRenderMap& renderMap ) const = 0;

    protected:     
           
        PLbool _isTransparent;
    
};


#endif 
