#ifndef __PL_I_GUIDE_SITE_H__ 
#define __PL_I_GUIDE_SITE_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"
#include "plMeshCutter.h"


// used in constructing the template base shape
class plEdgeIndices
{
    public:
    
        PLuint triangleIndex;
        PLuint vertexIndexA;
        PLuint vertexIndexB;
    
        plEdgeIndices() {}
        plEdgeIndices( PLuint t, PLuint vA, PLuint vB)  
            : triangleIndex(t), vertexIndexA(vA), vertexIndexB(vB) 
        {}
        
};


class plIGuideSite : public plModelSpecific,
                     public plRenderable
{    

    public:
        
        plBoundary boundary;

        plIGuideSite();
        plIGuideSite( PLuint _modelID, const plBoneAndCartilage &_model );
        plIGuideSite( PLuint _modelID, const plBoneAndCartilage &_model, const plBoundary &b );

        const plSeq<plTriangle> &templateBase() const { return _templateBase; };

        PLbool generateTemplateBase();

        void draw() const;

    private:
 
        plSeq<plTriangle>    _templateBase;                                         // for use in generating iGuides     
		
        plSeq<plEdgeIndices> _collectOutsideEdges         ();                       // helper to createTemplateBaseShape
        plSeq<plEdgeIndices> _collectOutsideEdgesUnsorted ();                       // helper to collectOutsideEdges
        plSeq<plEdgeIndices> _collectOutsideEdgesSort     ( plSeq<plEdgeIndices> ); // helper to collectOutsideEdges

};

#endif
