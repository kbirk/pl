#ifndef PL_DEFECT_SITE_H
#define PL_DEFECT_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plSpline.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"

class plDefectSite : public plModelSpecific,
                     public plRenderable
{
    public:

        plBoundary  boundary; 
        plSpline    spline;
             
        plDefectSite();
        plDefectSite( PLuint modelID, const plBoneAndCartilage& model );
        plDefectSite( PLuint modelID, const plBoneAndCartilage& model, const plSpline &s, const plBoundary &b );

        void extractRenderComponents( plRenderMap& renderMap ) const;  

        void draw() const;   
                
};


#endif

