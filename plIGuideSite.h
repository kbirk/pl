#ifndef __PL_I_GUIDE_SITE_H__ 
#define __PL_I_GUIDE_SITE_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"

class plIGuideSite : public plModelSpecific,
                     public plRenderable
{
    public:

        plBoundary     boundary;

        plIGuideSite();
        plIGuideSite( PLuint _modelID, const plBoneAndCartilage &_model );

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );

        void draw();
        
    private:
    
};

#endif
