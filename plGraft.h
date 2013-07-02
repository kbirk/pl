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
        PLuint      recipientModelID;
        plTransform harvestTransform;
        PLuint      harvestModelID;
    
        plGraft();

        const PLfloat    &heightOffset()  const { return _heightOffset; } 
        const PLfloat    &radius()        const { return  _radius; } 
        const PLfloat    &length()        const { return _length; } 
        const plVector3  &markDirection() const { return _markDirection; } 
        const plVector3  &markPosition()  const { return _markPosition; } 

        void heightOffset ( PLfloat heightOffset)           { _heightOffset = heightOffset; } 
        void radius       ( PLfloat radius)                 { _radius = radius; if (_radius < 0) _radius = 0; } 
        void length       ( PLfloat length)                 { _length = length; if (_length < 0) _length = 0; } 
        void markDirection( const plVector3 &markDirection) { _markDirection = markDirection; } 

        void computeTransforms();
        
        void draw() const;
        void drawGraft() const;
        void drawSelectionInterface() const;

        void updateCartilageMesh();
        void updateBoneMesh();
        void updateMarkPosition();
            
        void spinMark( PLfloat degrees );
    
        plCap findCap ( const plSeq<plTriangle> &triangles, const plVector3 &up );
        void  setCaps ();
        
        
    private:

        PLfloat     _heightOffset;
        PLfloat     _radius;
        PLfloat     _length;
        plVector3   _markDirection;
        plVector3   _markPosition;

        plMesh      _boneMesh;
        plMesh      _cartilageMesh;

        plCap       _cartilageCap;
        plCap       _boneCap;
        
        
        
        bool      triangleIntersection( const plTriangle &tri, plPoly &p ) const;
        plVector3 pointAtAngle( PLfloat theta ) const;
        PLfloat   angleOfPoint( const plVector3 &v ) const;
        plVector3 pointOnCircumference( const plVector3 &a, const plVector3 &b ) const;


};


#endif
