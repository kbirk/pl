#include "plEditor.h"

plEditor::plEditor()
    :    _isDraggingMenu( false ), _plan( nullptr )
{

}
        
void plEditor::attach( plPlan& plan )
{
    if ( _plan )
    {
        std::cout << "plEditor::attach() warning: plan already attached to editor" << std::endl;
    }
    _plan = &plan;
}


void plEditor::_clearEditable( plEditable& editable ) const
{
    editable._clearSelection();
}


void plEditor::_selectEditable( plEditable& editable, PLuint value ) const
{
    editable._isSelected = true;
    editable._selectedValue = value;
}