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
        
        ~plPlan();

        void importFile( plString filename );        
        void exportFile( plString filename );
                  
        plDefectSite&       defectSites( PLuint index )        { return *_defectSites[index]; }
        plDonorSite&        donorSites ( PLuint index )        { return *_donorSites[index];  }
        plIGuide&           iGuides    ( PLuint index )        { return *_iGuides[index];     }
        plGraft&            grafts     ( PLuint index )        { return *_grafts[index];      }   
        plBoneAndCartilage& models     ( PLuint index )        { return *_models[index];      }
              
        const plDefectSite&       defectSites( PLuint index ) const  { return *_defectSites[index]; }
        const plDonorSite&        donorSites ( PLuint index ) const  { return *_donorSites[index];  }
        const plIGuide&           iGuides    ( PLuint index ) const  { return *_iGuides[index];     }
        const plGraft&            grafts     ( PLuint index ) const  { return *_grafts[index];      }     
        const plBoneAndCartilage& models     ( PLuint index ) const  { return *_models[index];      }
        
        const plSeq<plDefectSite*>&       defectSites() const { return _defectSites; }
        const plSeq<plDonorSite*>&        donorSites()  const { return _donorSites; }
        const plSeq<plIGuide*>&           iGuides()     const { return _iGuides; }
        const plSeq<plGraft*>&            grafts()      const { return _grafts; }
        const plSeq<plBoneAndCartilage*>& models()      const { return _models; }
        
        void addDefectSite( PLuint modelIndex = 0 );
        void addDonorSite ( PLuint modelIndex = 0 );
        void addIGuide    ( PLuint modelIndex = 0 );

        void removeDefectSite( PLuint index);
        void removeDonorSite ( PLuint index);
        void removeIGuide    ( PLuint index);

        void drawElements() const;
        void drawModels()   const;

    private: 
    
        plSeq<plDefectSite*>       _defectSites;        
        plSeq<plDonorSite*>        _donorSites;
        plSeq<plIGuide*>           _iGuides;                
        plSeq<plGraft*>            _grafts;        
        plSeq<plBoneAndCartilage*> _models;     
};

std::ostream& operator << ( std::ostream& out, const plPlan &p );

#endif

