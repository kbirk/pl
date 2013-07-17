#ifndef __PL_RENDERABLE_H_
#define __PL_RENDERABLE_H_

#include "plCommon.h"
#include "plVector3.h"

class plRenderable
{
    
    public:
    
        plRenderable();  
        
        virtual void toggleVisibility();
		
    //protected:     
           
        PLbool isVisible;
    
};


#endif 
