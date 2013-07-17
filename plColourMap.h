#ifndef __PL_COLOUR_MAP_H__
#define __PL_COLOUR_MAP_H__

#include "plCommon.h"
#include "plVector3.h"

class plColourMap
{
    public:
        
        static plVector3 map(PLfloat k);

    private:

        plColourMap() {};

        static PLuint _colourMap[PL_COLOURMAP_SIZE][4]; 

};

#endif
