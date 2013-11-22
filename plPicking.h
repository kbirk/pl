#ifndef PL_PICKING_H
#define	PL_PICKING_H

#include "plCommon.h"
#include "plRenderResources.h"

class plPickingInfo 
{
    public:

	    PLint r;
	    PLint g;
	    PLint b;
	
	    plPickingInfo()
	        : r(-1), g(-1), b(-1)
	    {
	    }
	    
	    plPickingInfo( PLint red, PLint green, PLint blue) 
	        : r( red ), g( green ), b( blue )
	    {
	    }
};


namespace plPicking
{
    const plPickingInfo& previousPick();
    const plPickingInfo& pickPixel( PLuint x, PLuint y );
}

#endif

