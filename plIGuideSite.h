#ifndef PL_I_GUIDE_SITE_H
#define PL_I_GUIDE_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"
#include "plMeshCutter.h"
#include "plMeshExtruder.h"


class plIGuideSite : public plModelSpecific,
                     public plRenderable
{    

    public:
        
        plBoundary boundary;

        plIGuideSite();
        plIGuideSite( PLuint modelID, const plBoneAndCartilage& model );
        plIGuideSite( PLuint modelID, const plBoneAndCartilage& model, const plBoundary &b );

        void extractRenderComponents( plRenderMap& renderMap ) const;  
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
        const std::vector<plTriangle> &templateBase() const { return _templateBase; }

        PLbool generateTemplateBase();

        void draw() const;

    private:
 
        std::vector<plTriangle>  _templateBase;    // for use in generating iGuides

};

#endif
