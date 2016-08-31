#include "plPicking.h"

#include "plWindow.h"

namespace plPicking
{
    // store previous value read from picking buffer
    plPickingInfo _previousPick;


    const plPickingInfo& pickPixel(uint32_t x, uint32_t y)
    {
        // take into account the viewport x and y positions
        int32_t viewportX = x - plWindow::viewportX();
        int32_t viewportY = y - plWindow::viewportY();

        // read pixel from color buffer
        auto fbo = plRenderResources::fbos(PL_MAIN_FBO);
        auto pick = fbo->readPixel<int32_t>(GL_COLOR_ATTACHMENT4, viewportX, viewportY);

        // store pixel value
        _previousPick = plPickingInfo(pick.r, pick.g, pick.b);

        // return value
        return _previousPick;
    }


    const plPickingInfo& previousPick()
    {
        return _previousPick;
    }

}
