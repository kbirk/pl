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
		
		plMatrix44 getMatrix();	
		
	    void setFOV(PLfloat fov);	
	    void setNear(PLfloat nearPlane);	
	    void setFar(PLfloat farPlane);	    
	    void setAspectRatio(PLfloat aspect_ratio);
	    
	
	private:
				
		PLfloat _fov;
		PLfloat _aspect;
		PLfloat _near;
		PLfloat _far;
				
};

#endif
