#ifndef __PL_PLAN_H__
#define __PL_PLAN_H__

#include "pl.h"
#include "plError.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plCSV.h"
#include "plGraft.h"
#include "plRenderable.h"
#include "plModel.h"
#include "plBoundary.h"
#include "plSpline.h"
#include "plString.h"
#include "plDonorSite.h"
#include "plIGuide.h"

class plPlan : public plRenderable
{
    public:

        plString         date;
        plString         directory; 
   
        plPlan();
        plPlan( plString filename );

        void defaultInit();
        void readFile( plString filename );

        //plBoundary &getBoundaryReference(PLuint type, PLuint id);
              
        void draw();
        
        //void outputForManuela();
        
        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
    //private: eventually make these member variables private
    
        plSeq<plSpline>    _defectSplines;        
        plSeq<plDonorSite> _donorRegions;
        plSeq<plIGuide>    _iGuides;                
        plSeq<plGraft>     _grafts;      
};

std::ostream& operator << ( std::ostream& out, const plPlan &p );

#endif

