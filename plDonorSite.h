#ifndef __PL_DONOR_SITE_H__
#define __PL_DONOR_SITE_H__

#include "pl.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plTriangle.h"
#include "plRenderable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"


class plDonorSite : public plRenderable
{
    public:

        plBoundary boundary; 
                
        plDonorSite();

        void init();
        void readFromCSV( const plSeq<plString> &row );
        void draw() const;      

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
    private:
    
        PLuint _modelID;
};


#endif

