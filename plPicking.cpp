#include "plPicking.h"


namespace plPicking
{
    // store previous value read from picking buffer
    plPickingInfo _previousPick;
      
       
    const plPickingInfo& pickPixel( PLuint x, PLuint y )
    {   
        // transform window coords to viewport / fbo texture coords
        GLint viewport[4];
	    glGetIntegerv( GL_VIEWPORT, viewport );   
        PLuint viewportX = x - viewport[0];
        PLuint viewportY = y - viewport[1];
    
        plPixel<PLint> pick = plRenderResources::fbos( PL_MAIN_FBO )->readPixel<PLint>( GL_COLOR_ATTACHMENT4, viewportX, viewportY );

        _previousPick = plPickingInfo( pick.r, pick.g, pick.b );

        std::cout << "picking: " << _previousPick.r << " " << _previousPick.g << " " << _previousPick.b << "\n"; 
    
        return _previousPick;   
    }


    const plPickingInfo& previousPick()
    {
        return _previousPick;
    }
    
}






