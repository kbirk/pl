#ifndef __PL_GRAFT_H__
#define __PL_GRAFT_H__

#include "pl.h"
#include "plRenderable.h"
#include "plVector3.h"
#include "plTriangle.h"
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

        PLfloat   height_offset;
        PLfloat   radius;
        PLfloat   length;
        plVector3 markDirection;

        plTransform recipientTransform;
        //PLuint    recipientModelID;
        plTransform harvestTransform;
        //PLuint    harvestModelID;
    
        plCap cartilageCap;
        plCap boneCap;
  
        plMesh boneMesh;
        plMesh cartilageMesh;
  
        plGraft();

        void computeTransforms();
        
        void draw() const;
        void drawGraft() const;
        void drawSelectionInterface() const;

        void spinMark( PLfloat degrees );
    
        plCap findCap ( const plSeq<plTriangle> &triangles, const plVector3 &up );
        void setCaps  ( const plSeq<plTriangle> &cartilageTriangles, const plSeq<plTriangle> &boneTriangles );
        
        void updateCartilageMesh();
        void updateBoneMesh();

        bool      triangleIntersection( const plTriangle &tri, plPoly &p ) const;
        plVector3 pointAtAngle( PLfloat theta ) const;
        PLfloat   angleOfPoint( const plVector3 &v ) const;
        plVector3 pointOnCircumference( const plVector3 &a, const plVector3 &b ) const;
};


#endif
