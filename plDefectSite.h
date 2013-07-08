#ifndef _PL_DEFECT_SITE_H_
#define _PL_DEFECT_SITE_H_

#include "pl.h"
#include "plVector3.h"
#include "plSpline.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"

class plDefectSite : public plRenderable
{
    public:

        plBoundary  boundary, corners; 
        plSpline    spline;
             
        plDefectSite();

        void readFromCSV( const plSeq<plString> &row );
             
        void computeSpline( const plSeq<plBoneAndCartilage> &models );
        
        void draw() const;   

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
                
    private:
    
        PLuint _modelID; 
              
        //void _drawCornersSelectionInterface() const;      
                
};


#endif

