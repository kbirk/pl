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
#include "plGraft.h"
#include "plBoundaryIntersection.h"

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

        PLfloat        thickness;
        plBoundary     boundary;
        plSeq<PLuint>  graftIndices;
        plSeq<plKWire> kwires;

        // individual pieces that are to be either boolean'ed together or exported to some other program
        plSeq<plTriangle> templateBase;
        plSeq<plTriangle> recipientSleeve;
        plSeq<plTriangle> recipientHole;
        plSeq<plTriangle> harvestSleeve;
        plSeq<plTriangle> harvestHole;

        plIGuide();
        plIGuide( PLuint _modelID, const plBoneAndCartilage &_model );

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );
        
        void createTemplateBaseShape(const plSeq<plTriangle> &cartilageTris, const plGraft &graft);

        void draw();
};


#endif

