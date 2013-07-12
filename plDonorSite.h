#ifndef __PL_DONOR_SITE_H__
#define __PL_DONOR_SITE_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"

class plDonorSite : public plRenderable
{
    public:

        plBoundary boundary; 
                
        plDonorSite();

        void readFromCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models );

        void draw() const;      

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
    private:
    
        PLuint                   _modelID;
        const plBoneAndCartilage *model;  
};


#endif

