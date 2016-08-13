#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderComponent.h"

class plArthroViewable
{

    public:

        plArthroViewable();

        PLbool inArthroView() const { return _inArthroView; }

        virtual void toggleArthroView();

    protected:

        PLbool _inArthroView;

};
