#ifndef __PL_PLAN_H__
#define __PL_PLAN_H__

#include "plCommon.h"
#include "plString.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plCSV.h"
#include "plRenderable.h"
#include "plGraft.h"
#include "plBoneAndCartilage.h"
#include "plDefectSite.h"
#include "plDonorSite.h"
#include "plIGuide.h"

class plPlan : public plRenderable
{
    public:

        plPlan( plString filename );        
        plPlan( int argc, char **argv ); 
        
        void importFile( plString filename );        
        void exportFile( plString filename );
          
        /*
        plDefectSite& defectSites( PLuint index );
        plDonorsite&  donorSites ( PLuint index );
        plIGuides&    iGuides    ( PLuint index );
        plGraft&      grafts     ( PLuint index );
        */
        
        void addDefectSite( PLuint modelIndex = 0 );
        void addDonorSite ( PLuint modelIndex = 0 );
        void addIGuide    ( PLuint modelIndex = 0 );

        void removeDefectSite( PLuint index);
        void removeDonorSite ( PLuint index);
        void removeIGuide    ( PLuint index);

        void drawElements() const;
        void drawModels()   const;

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
    //private: eventually make these member variables private, 
            // use an indexed accessor function returning a non-const reference, so individual 
            // elements can be editted, but plSeq functions cannot be called
        plSeq<plDefectSite>       _defectSites;        
        plSeq<plDonorSite>        _donorSites;
        plSeq<plIGuide>           _iGuides;                
        plSeq<plGraft>            _grafts;        
        plSeq<plBoneAndCartilage> _models;     
};

std::ostream& operator << ( std::ostream& out, const plPlan &p );

#endif

