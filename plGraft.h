#ifndef __PL_GRAFT_H__
#define __PL_GRAFT_H__

#include "pl.h"
#include "plRenderable.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plModel.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plState.h"
#include "plTransform.h"
#include "plMesh.h"


class plPoly 
{

    public:

        plSeq<plVector3> vertices;
        plVector3        normal;

        plPoly() {}
};


class plPointAndAngle 
{
    public:
        
        PLfloat   angle;
        plVector3 point;
        
        plPointAndAngle() {}
        plPointAndAngle( const PLfloat &a, const plVector3 &v ) 
        { 
            angle = a; 
            point = v; 
        }
}; 


class plCap 
{

    public:

        plSeq<plPoly>           polys;	    // polygons of the cap
        plSeq<plPointAndAngle>  perimeter;  // perimeter vertices, ordered CCW from above (?)
    
        plCap() {}
};


class plGraft : public plRenderable
{

    public:

        plTransform recipientTransform;        
        plTransform harvestTransform;
            
        plGraft();
        
        void init();
        void readFromCSV( const plSeq<plString> &row );

        PLfloat heightOffset() const { return _heightOffset; } 
        PLfloat radius()       const { return _radius; } 
        PLfloat length()       const { return _length; } 

        void adjustHeightOffset ( PLfloat adjustment) { _heightOffset += adjustment; } 
        void adjustRadius       ( PLfloat adjustment) { _radius += adjustment; if (_radius < 0) _radius = 0; } 
        void adjustLength       ( PLfloat adjustment) { _length += adjustment; if (_length < 0) _length = 0; } 

        //void computeTransforms();
        
        void setCaps();
        
        void draw() const;
        
        void updateCartilageMesh();
        void updateBoneMesh();
        void updateMarkPosition();
            
        void spinMark( PLfloat degrees );
         
        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
         
    private:

        PLuint     _recipientModelID;
        PLuint     _harvestModelID;
        
        PLfloat    _heightOffset;
        PLfloat    _radius;
        PLfloat    _length;
        plVector3  _markDirection;
        plVector3  _markPosition;

        plMesh     _boneMesh;
        plMesh     _cartilageMesh;

        plCap      _cartilageCap;
        plCap      _boneCap;
        
        void      _drawGraft() const;
        void      _drawSelectionInterface() const;
        
        plCap     _findCap              ( const plSeq<plTriangle> &triangles );
        bool      _triangleIntersection ( const plTriangle &tri, plPoly &p ) const;
        plVector3 _pointAtAngle         ( PLfloat theta ) const;
        PLfloat   _angleOfPoint         ( const plVector3 &v ) const;
        plVector3 _pointOnCircumference ( const plVector3 &a, const plVector3 &b ) const;


};

bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b );

#endif
