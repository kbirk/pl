#include "plPicking.h"

namespace plPicking
{
    // store previous value read from picking buffer
    plPickingInfo _previousPick;


    const plPickingInfo& pickPixel(uint32_t x, uint32_t y)
    {
        // transform window coords to viewport / fbo texture coords
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        uint32_t viewportX = x - viewport[0];
        uint32_t viewportY = y - viewport[1];

        // read pixel from color buffer
        plPixel<int32_t> pick = plRenderResources::fbos(PL_MAIN_FBO)->readPixel<int32_t>(GL_COLOR_ATTACHMENT4, viewportX, viewportY);

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
