#ifndef __PL_PROJECTION_H__
#define __PL_PROJECTION_H__

#include "pl.h"
#include "plMatrix44.h"
#include "plPickingTexture.h"

class plProjection
{
    public:                    

        plProjection( PLfloat aspectRatio );
		plProjection( PLfloat fov, PLfloat aspectRatio, PLfloat nearPlane, PLfloat farPlane );		
		
		plMatrix44 matrix() const              { return plMatrix44( _fov, _aspect, _nearPlane, _farPlane ); }	
		
	    void fov (PLfloat fov)                 { _fov = fov;               }	
	    void nearPlane(PLfloat nearPlane)      { _nearPlane = nearPlane;   }
	    void farPlane(PLfloat farPlane)        { _farPlane  = farPlane;    }    
	    void aspectRatio(PLfloat aspectRatio)  { _aspect    = aspectRatio; }

	private:
				
		PLfloat _fov;
		PLfloat _aspect;
		PLfloat _nearPlane;
		PLfloat _farPlane;
				
};

#endif
