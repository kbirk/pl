#pragma once

#include "plCommon.h"
#include "plRenderComponent.h"

class plEditable {
public:
    plEditable();

    bool isSelected() const { return _isSelected; }

    // only editors can see the private variables
    friend class plEditor;

protected:
    uint32_t _selectedValue;
    bool _isSelected;

    virtual void _clearSelection();
};
