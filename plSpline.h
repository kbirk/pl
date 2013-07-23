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

#include <future>

class plSpline : public plBoundary
{
    public:

        plSpline();
        plSpline( const plBoneAndCartilage &model );

        const plSeq<plTriangle> &triangles() const { return _triangles; }

        void   draw() const;           

        void   importCSV( const plSeq<plString> &row, const plBoneAndCartilage &model );

        PLuint addPointAndNormal (const plVector3 &point, const plVector3 &normal);
        void   movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal);
        void   removePointAndNormal( PLuint index );

        plIntersection rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;        

    private:
    
        PLtime            _lastUpdate;
        plColourMesh      _surfaceMesh;
        plSeq<plTriangle> _triangles;
        plOctree          _octree;

        plSeq<plVector3> _averageCornerNormals() const;
        
        void _getMinMax(plVector3 &min, plVector3 &max) const;
                
        void _computeTangents( plSeq<PLfloat> &st, plSeq<PLfloat> &tt, const plSeq<plVector3> &p, const plSeq<plVector3> &n ) const;
        void _computeHermite();

        void _drawSelectionInterface() const;
           
        PLuint _timeSinceLastUpdate();   
};


#endif

