#ifndef SPLINE_H
#define SPLINE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plColourMesh.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plColourMap.h"
#include "plModel.h"

class plSpline : public plBoundary
{
    public:

        plSpline();
        plSpline( const plBoneAndCartilage &model );

        void   draw() const;           

        void   importCSV( const plSeq<plString> &row, const plBoneAndCartilage &model );

        PLuint addPointAndNormal (const plVector3 &point, const plVector3 &normal);
        void   movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal);
        void   removePointAndNormal( PLuint index );

    private:
    
        PLtime          _lastUpdate;
        plColourMesh    _surfaceMesh;
        plSeq<plVector3> _s, _t;  

        void _computeHermite();

        void _drawSelectionInterface() const;
           
        PLuint _timeSinceLastUpdate();   
};


#endif

