#include "plArthroViewable.h"


plArthroViewable::plArthroViewable()
    :   _inArthroView( false )
{
}


void plArthroViewable::toggleArthroView()
{
    _inArthroView = !_inArthroView;
}
