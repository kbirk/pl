#ifndef __PL_I_GUIDE_SITE_H__ 
#define __PL_I_GUIDE_SITE_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"
#include "plMeshCutter.h"

class plIGuideSite : public plModelSpecific,
                     public plRenderable
{
    private:
        // used in constructing the template base shape
        class edgeIndices
        {
            public:
                edgeIndices() {}
                edgeIndices(PLuint t, PLuint vA, PLuint vB) : triangleIndex(t), vertexIndexA(vA), vertexIndexB(vB) {}
                PLuint triangleIndex;
                PLuint vertexIndexA;
                PLuint vertexIndexB;
        };

        plSeq<edgeIndices> collectOutsideEdges();                       // helper to createTemplateBaseShape
        plSeq<edgeIndices> collectOutsideEdgesUnsorted();               // helper to collectOutsideEdges
        plSeq<edgeIndices> collectOutsideEdgesSort(plSeq<edgeIndices>); // helper to collectOutsideEdges

    public:

        plSeq<plTriangle>   templateBase;   // for use in generating iGuides
        plBoundary          boundary;

        plIGuideSite();
        plIGuideSite( PLuint _modelID, const plBoneAndCartilage &_model );

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );

        void draw();

        void createTemplateBaseShape( plSeq<plTriangle> &cartilageTris );

};

#endif
