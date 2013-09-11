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
        plSpline( const plModel &cartilage );
        plSpline( const plSeq<plString> &row, const plModel &cartilage );

        const plSeq<plTriangle> &triangles() const { return _triangles; }

        void   draw() const;           

        PLuint addPointAndNormal   ( const plVector3 &point, const plVector3 &normal );
        void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal );
        void   removePointAndNormal( PLuint index );

        plVector3 getAverageNormalOverCorners();

        void   clear();

    private:
    
        const plModel    *_cartilage;
    
        PLtime            _lastUpdate;
        plColourMesh      _surfaceMesh;
        plSeq<plTriangle> _triangles;

        plSeq<plVector3>  _averageCornerNormals() const;
              
        void    _computeTangents( plSeq<PLfloat> &st, plSeq<PLfloat> &tt, const plSeq<plVector3> &p, const plSeq<plVector3> &n ) const;
        void    _computeHermite();
                
        PLuint  _timeSinceLastUpdate();  
        
        //void    _drawSelectionInterface() const;  
};


#endif

