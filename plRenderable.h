#ifndef __PL_RENDERABLE_H_
#define __PL_RENDERABLE_H_

#include "pl.h"
#include "plVector3.h"

class plRenderable
{
    
    public:
    
        plRenderable();  
        
        virtual void toggleVisibility();
        virtual PLbool isVisible() const;
		
    protected:     
           
        PLbool _isVisible;
    
};


#endif 
