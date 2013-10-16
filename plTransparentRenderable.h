#ifndef PL_TRANSPARENT_RENDERABLE_H
#define PL_TRANSPARENT_RENDERABLE_H

#include "plCommon.h"
#include "plRenderable.h"


class plTransparentRenderable : public plRenderable
{
    
    public:
    
        plTransparentRenderable();  
        
        PLbool isTransparent() const;
        virtual void toggleVisibility();	
        virtual void toggleTransparency();	
		virtual void setVisible();
		virtual void setTransparent();
		virtual void setInvisible();
		virtual void saveState();
		virtual void loadState();

    protected:     
           
        PLbool _isTransparent;
    
};


#endif 
