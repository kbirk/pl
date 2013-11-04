#ifndef PL_I_GUIDE_SITE_H
#define PL_I_GUIDE_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"
#include "plMeshCutter.h"
#include "plMeshExtruder.h"


class plIGuideSite : public plModelSpecific,
                     public plRenderable
{    

    public:
        
        plBoundary boundary;

        plIGuideSite();
        plIGuideSite( PLuint _modelID, const plBoneAndCartilage &_model );
        plIGuideSite( PLuint _modelID, const plBoneAndCartilage &_model, const plBoundary &b );

        const std::vector<plTriangle> &templateBase() const { return _templateBase; }

        PLbool generateTemplateBase();

        void draw() const;

    private:
 
        std::vector<plTriangle>    _templateBase;                                         // for use in generating iGuides

};

#endif
