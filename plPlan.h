#ifndef __PL_PLAN_H__
#define __PL_PLAN_H__

#include "pl.h"
#include "plVector3.h"
#include "plCSV.h"
#include "plGraft.h"
#include "plRenderable.h"
#include "plModel.h"
#include "plBoundary.h"
#include "plSpline.h"
#include "plString.h"

class plKWire 
{
    public:
        
        plVector3 position;
        plVector3 direction;
  
        plKWire() 
        {
        }
};


class plIGuide : public plRenderable
{
    public:

        plBoundary   boundary;
        plSeq<int>   graftIndices;
        plSeq<plKWire> kwires;

        plIGuide() {}

        void draw() 
        {
            boundary.draw();
        }
};
 

class plPlan : public plRenderable
{
    public:

        plString         date;
        plString         directory;
        plString         baseFilename;   
        
        plSeq<plSpline>   _defectSplines;        
        plSeq<plBoundary> _donorRegions;
        plSeq<plIGuide>   _iGuides;
                
        plSeq<plGraft>    _grafts;      
               
        plPlan();
        plPlan( plString filename );

        void defaultInit();
        void readFile( plString filename );

        plBoundary &getBoundaryReference(PLuint type, PLuint id);
              
        void draw();
        
        //void outputForManuela();
};

std::ostream& operator << ( std::ostream& out, plPlan const &p );

#endif

