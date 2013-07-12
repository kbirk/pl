#ifndef _PL_DEFECT_SITE_H_
#define _PL_DEFECT_SITE_H_

#include "plCommon.h"
#include "plVector3.h"
#include "plSpline.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"
#include "plBoneAndCartilage.h"

class plDefectSite : public plRenderable
{
    public:

        plBoundary  boundary; 
        plSpline    spline;
             
        plDefectSite();

        void readFromCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models );

        void draw() const;   

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
                
    private:
          
        PLuint                    _modelID;
        const plBoneAndCartilage *_model;  
    
                
};


#endif

