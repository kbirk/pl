#ifndef PL_PROJECTION_H
#define PL_PROJECTION_H

#include "plCommon.h"
#include "plMatrix44.h"

#define PL_ASPECT_RATIO                          ( 1280.0f / 720.0f ) 
#define PL_NEAR_PLANE                            10.0f
#define PL_FAR_PLANE                             15000.0f
#define PL_FIELD_OF_VIEW                         7.0f

class plProjection
{
    public:                    

        plProjection( PLfloat aspectRatio );
		plProjection( PLfloat fov, PLfloat aspectRatio, PLfloat nearPlane, PLfloat farPlane );		
		
		plMatrix44 matrix() const                { return plMatrix44( _fov, _aspect, _nearPlane, _farPlane ); }	
		
	    void fov        ( PLfloat fov         )  { _fov = fov;               }	
	    void nearPlane  ( PLfloat nearPlane   )  { _nearPlane = nearPlane;   }
	    void farPlane   ( PLfloat farPlane    )  { _farPlane  = farPlane;    }    
	    void aspectRatio( PLfloat aspectRatio )  { _aspect    = aspectRatio; }

	private:
				
		PLfloat _fov;
		PLfloat _aspect;
		PLfloat _nearPlane;
		PLfloat _farPlane;
				
};

#endif
