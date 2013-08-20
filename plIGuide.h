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
#include "plPolygon.h"
#include "plIGuideSite.h"

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

    private:

        plSeq<plTriangle>  createTemplatePieceTransformed ( const plSeq<plTriangle> &baseTriObject ,
                                                            const plMatrix44  &plugTransform,
                                                            const PLdouble    &zOffset,
                                                            const plVector3   &scale,
                                                            const PLdouble    &keyTranslationXAxis,
                                                            const PLdouble    &keyRotationZAxis );

    public:

        PLfloat                 thickness;

        plSeq<plIGuideSite>     *sites;
        PLint                   siteIndex;

        plSeq<plGraft>          *grafts;
        plSeq<PLint>            recipientIndices;      // pointer to the plan's grafts
        plSeq<PLint>            harvestIndices;

        plSeq<plKWire>          *kWires;
        plSeq<PLint>            kWireIndices;

        plIGuide();
        plIGuide( PLuint _modelID, const plBoneAndCartilage &_model );

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );

        PLbool generateMeshes();

        void draw();
};


#endif

