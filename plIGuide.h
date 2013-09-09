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

class plIGuideModel : public plModel
{
    public:

        plSeq<plTriangle> triangles;
        plString          filename;

        plIGuideModel(const plString &str, const plSeq<plTriangle> &tris) : plModel(tris,0), triangles(tris), filename(str) {}
};

class plKWire 
{
    public:

        PLfloat   thickness;
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

        plString prepareFilenameWithVariables( PLbool add, PLchar type, PLint graftIndex, const plString &pieceName );


    public:

        // ATTRIBUTES
        // geometry data
        PLfloat                 thickness;
        PLfloat                 printerOffset;

        plSeq<plModel*>         iGuideModelsToAdd;
        plSeq<plModel*>         iGuideModelsToSubtract;

        // pointers to data needed to construct this thing
        plSeq<plIGuideSite*>    *sites;
        PLint                   siteIndex;

        plSeq<plGraft*>         *grafts;
        plSeq<PLint>            recipientIndices;      // pointer to the plan's grafts
        plSeq<PLint>            harvestIndices;

        plSeq<plKWire*>         *kWires;
        plSeq<PLint>            kWireIndices;


        // MEMBERS
        // constructors
        plIGuide();
        plIGuide( PLuint _modelID, const plBoneAndCartilage &_model );

        // accessors (used to not have to dereference plSeq pointers and overall make code a bit more readable)
        plIGuideSite &site           ()                 { return *((*sites)[siteIndex]); }
        plGraft      &recipientGraft (PLint arrayIndex) { return *((*grafts)[recipientIndices[arrayIndex]]); }
        plGraft      &harvestGraft   (PLint arrayIndex) { return *((*grafts)[harvestIndices[arrayIndex]]); }
        plKWire      &kWire          (PLint arrayIndex) { return *((*kWires)[arrayIndex]); }

        // I/O
        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );

        // core functionality
        PLbool generateIGuideModels ();
        PLbool exportIGuideModels   (const plString &directory);
        void   clearIGuideModels    ();

        // rendering
        void draw();
};


#endif

