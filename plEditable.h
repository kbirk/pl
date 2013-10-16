#ifndef PL_EDITABLE_H
#define PL_EDITABLE_H

#include "plCommon.h"

class plEditable
{   
    public:

        plEditable();  
        
        // only editors can see the private variables
        friend class plGraftEditor;
        friend class plBoundaryEditor;

    protected:     
           
        PLuint _selectedValue;
        PLbool _isSelected;
        
        virtual void _clearSelection();
};


#endif 
