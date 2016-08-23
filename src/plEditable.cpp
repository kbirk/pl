#include "plEditable.h"

plEditable::plEditable()
{
    _clearSelection();
}

void plEditable::_clearSelection()
{
    _isSelected = false;
    _selectedValue = -1;
}
