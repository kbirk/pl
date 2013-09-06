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
#include "plIGuideSite.h"
#include "plIGuide.h"

class plPlan : public plRenderable
{
    public:

        plPlan( plString filename );        
        plPlan( int argc, char **argv ); 
        
        ~plPlan();

        void importFile( plString filename );        
        void exportFile( plString filename );
         
        // non-const reference getters          
        plDefectSite&       defectSites( PLuint index )        { return *_defectSites[index]; }
        plDonorSite&        donorSites ( PLuint index )        { return *_donorSites[index];  }
        plIGuideSite&       iGuideSites( PLuint index )        { return *_iGuideSites[index]; }
        plIGuide&           iGuides    ( PLuint index )        { return *_iGuides[index];     }
        plGraft&            grafts     ( PLuint index )        { return *_grafts[index];      }   
        plBoneAndCartilage& models     ( PLuint index )        { return *_models[index];      }
           
        // const reference getters      
        const plDefectSite&       defectSites( PLuint index ) const  { return *_defectSites[index]; }
        const plDonorSite&        donorSites ( PLuint index ) const  { return *_donorSites[index];  }
        const plIGuideSite&       iGuideSites( PLuint index ) const  { return *_iGuideSites[index]; }
        const plIGuide&           iGuides    ( PLuint index ) const  { return *_iGuides[index];     }
        const plGraft&            grafts     ( PLuint index ) const  { return *_grafts[index];      }     
        const plBoneAndCartilage& models     ( PLuint index ) const  { return *_models[index];      }
        
        // const array getters (gives access to array without allowing control to add, remove, etc)
        const plSeq<plDefectSite*>&       defectSites() const { return _defectSites; }
        const plSeq<plDonorSite*>&        donorSites()  const { return _donorSites;  }
        const plSeq<plIGuideSite*>&       iGuideSites() const { return _iGuideSites; }
        const plSeq<plIGuide*>&           iGuides()     const { return _iGuides;     }
        const plSeq<plGraft*>&            grafts()      const { return _grafts; }
        const plSeq<plBoneAndCartilage*>& models()      const { return _models; }
        
        void addDefectSite( PLuint modelIndex = 0 );
        void addDonorSite ( PLuint modelIndex = 0 );
        void addIGuideSite( PLuint modelIndex = 0 );
        void addGraft     ( const plPlug &h, const plPlug &r, PLfloat radius, PLfloat length = 15.0f, PLfloat heightOffset = 0.0f );

        void removeDefectSite( PLuint index );
        void removeDonorSite ( PLuint index );
        void removeIGuideSite( PLuint index );
        void removeGraft     ( PLuint index );

        void generateIGuide  ( PLuint index );
        
        void drawElements() const;
        void drawModels()   const;

        void clear();

    private: 
    
        plSeq<plDefectSite*>       _defectSites;        
        plSeq<plDonorSite*>        _donorSites;
        plSeq<plIGuideSite*>       _iGuideSites;  
        plSeq<plIGuide*>           _iGuides;                
        plSeq<plGraft*>            _grafts;        
        plSeq<plBoneAndCartilage*> _models;     
};

std::ostream& operator << ( std::ostream& out, const plPlan &p );

#endif

