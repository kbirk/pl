#ifndef __PL_GRAFT_H__
#define __PL_GRAFT_H__

#include "pl.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plModel.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plTransform.h"
#include "plMesh.h"
#include "plPlan.h"

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

/*
class plPlug
{
    public:

        plPlug() {}

        PLuint      _modelID
        plTransform _transform;
}
*/

class plGraft : public plRenderable,
                public plEditable
{

    public:

        plTransform recipientTransform;        
        plTransform harvestTransform;
            
        plGraft();
        
        void init( const plSeq<plBoneAndCartilage> &models );
        
        void readFromCSV( const plSeq<plString> &row );

        PLfloat heightOffset()     const { return _heightOffset; } 
        PLfloat radius()           const { return _radius; } 
        PLfloat length()           const { return _length; } 
        
        PLuint  recipientModelID() const { return _recipientModelID; }
        PLuint  harvestModelID()   const { return _harvestModelID; }

        void adjustHeightOffset ( PLfloat adjustment) { _heightOffset += adjustment; } 
        void adjustRadius       ( PLfloat adjustment) { _radius += adjustment; if (_radius < 0) _radius = 0; _updateCartilageMesh(); _updateBoneMesh(); } 
        void adjustLength       ( PLfloat adjustment) { _length += adjustment; if (_length < 0) _length = 0; _updateCartilageMesh(); _updateBoneMesh(); } 

        void translateHarvest  ( const plVector3 &translation );
        void translateRecipient( const plVector3 &translation );

        void spinMark( PLfloat degrees );
         
        void draw() const;

void      setCaps               ( const plSeq<plBoneAndCartilage> &models );

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
        
        void      _setBoneColour() const;
        void      _setCartilageColour() const;
                
        void      _drawGraft() const;
               
        plCap     _findCap              ( const plSeq<plTriangle> &triangles );
        bool      _triangleIntersection ( const plTriangle &tri, plPoly &p ) const;
        plVector3 _pointAtAngle         ( PLfloat theta ) const;
        PLfloat   _angleOfPoint         ( const plVector3 &v ) const;
        plVector3 _pointOnCircumference ( const plVector3 &a, const plVector3 &b ) const;

        void      _updateCartilageMesh();
        void      _updateBoneMesh();
        void      _updateMarkPosition();

        void      _translate( plTransform &transform, PLuint modelID, const plVector3 &translation );
};

bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b );

#endif
