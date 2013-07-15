#ifndef _PL_DEFECT_SITE_H_
#define _PL_DEFECT_SITE_H_

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plSpline.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"

class plDefectSite : public plModelSpecific,
                     public plRenderable
{
    public:

        plBoundary  boundary; 
        plSpline    spline;
             
        plDefectSite();
        plDefectSite( PLuint _modelID, const plBoneAndCartilage &_model );

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models );

        void draw() const;   
                
};


#endif

