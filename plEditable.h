#ifndef PL_EDITABLE_H
#define PL_EDITABLE_H

#include "plCommon.h"
#include "plRenderComponent.h"

class plEditable
{
    public:

        plEditable();

        PLbool isSelected() const { return _isSelected; }

        // only editors can see the private variables
        friend class plEditor;

    protected:

        PLuint _selectedValue;
        PLbool _isSelected;

        virtual void _clearSelection();
};



#endif
