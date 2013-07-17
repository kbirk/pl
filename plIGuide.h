#ifndef __PL_I_GUIDE_H__ 
#define __PL_I_GUIDE_H__

#include "plCommon.h"
#include "plModelSpecific.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"

class plKWire 
{
    public:
        
        plVector3 position;
        plVector3 direction;
  
        plKWire() {}
};

class plIGuide : public plModelSpecific,
                 public plRenderable
{
    public:

        plBoundary     boundary;
        plSeq<PLuint>  graftIndices;
        plSeq<plKWire> kwires;

        plIGuide();
        plIGuide( PLuint _modelID, const plBoneAndCartilage &_model );

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );
        
        void draw();
};


#endif

