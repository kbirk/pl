#ifndef PL_DEFECT_SITE_H
#define PL_DEFECT_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plMeshSpecific.h"
#include "plSpline.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"

class plDefectSite : public plRenderable
{
    public:

        plSpline    spline;
        plBoundary  boundary; 
                    
        plDefectSite();
        plDefectSite( const plMesh& mesh );
        plDefectSite( const plSpline& spline, const std::vector<plString> &row );

        void extractRenderComponents( plRenderMap& renderMap ) const;  
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;  
        
        void recastBoundary();
        
    private:
    
                 
};


#endif

