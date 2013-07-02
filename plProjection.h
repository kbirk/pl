#ifndef __PL_PROJECTION_H__
#define __PL_PROJECTION_H__

#include "pl.h"
#include "plMatrix44.h"
#include "plPickingTexture.h"

class plProjection
{
    public:                    

        plProjection(PLfloat aspect_ratio);
		plProjection(PLfloat fov, PLfloat aspect_ratio, PLfloat near_plane, PLfloat far_plane);		
		
		plMatrix44 matrix()                     { return plMatrix44( _fov, _aspect, _near, _far ); }	
		
	    void fov(PLfloat fov)                   { _fov = fov;              }	
	    void near(PLfloat nearPlane)            { _near = nearPlane;      }
	    void far(PLfloat farPlane)              { _far = farPlane;        }    
	    void aspectRatio(PLfloat aspectRatio)  { _aspect = aspectRatio; }

	private:
				
		PLfloat _fov;
		PLfloat _aspect;
		PLfloat _near;
		PLfloat _far;
				
};

#endif
