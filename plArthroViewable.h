#ifndef PL_ARTHRO_VIEWABLE_H
#define PL_ARTHRO_VIEWABLE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderComponent.h"

class plArthroViewable
{
    
    public:
    
        plArthroViewable();  
        
        virtual void toggleArthroView();	
        
    protected:     
           
        PLbool _inArthroView;
    
};


#endif 
