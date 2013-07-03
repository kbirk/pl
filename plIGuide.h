#ifndef __PL_I_GUIDE_H__ 
#define __PL_I_GUIDE_H__

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"

class plKWire 
{
    public:
        
        plVector3 position;
        plVector3 direction;
  
        plKWire() {}
};

class plIGuide : public plRenderable
{
    public:

        plBoundary     boundary;
        plSeq<PLuint>  graftIndices;
        plSeq<plKWire> kwires;

        plIGuide() {}

        void readFromCSV( const plSeq<plString> &row );
        
        void draw();
};


#endif

