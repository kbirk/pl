#ifndef __PL_GRAFT_H__
#define __PL_GRAFT_H__

#include "plCommon.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plDraw.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"
#include "plPlug.h"
#include "plPolygon.h"

//class plPlan;

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

        plSeq<plPolygon>        polys;      // polygons of the cap
        plSeq<plPointAndAngle>  perimeter;  // perimeter vertices, ordered CCW from above (?)
    
        plCap() {}
};


class plGraft : public plRenderable,
                public plEditable
{

    public:
   
        plGraft();
        
        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models );

        PLfloat          heightOffset()  const { return _heightOffset; } 
        PLfloat          radius()        const { return _radius; } 
        PLfloat          length()        const { return _length; } 
        const plVector3 &markDirection() const { return _markDirection; } 
        
        void adjustHeightOffset ( PLfloat adjustment) { _heightOffset += adjustment; } 
        void adjustRadius       ( PLfloat adjustment) { _radius += adjustment; if (_radius < 0) _radius = 0; _updateCartilageMesh(); _updateBoneMesh(); } 
        void adjustLength       ( PLfloat adjustment) { _length += adjustment; if (_length < 0) _length = 0; _updateCartilageMesh(); _updateBoneMesh(); } 

        const plTransform &transform( PLuint type ) const;

        void translate  ( PLuint type, const plVector3 &translation );
        void translateX ( PLuint type, PLfloat distance, const plVector3 &planeNormal );
        void translateZ ( PLuint type, PLfloat distance, const plVector3 &planeNormal );
        void rotate     ( PLuint type, const plVector3 &axis, PLfloat angleDegrees );
        void spinMark   ( PLfloat degrees );
         
        void draw() const;

        //friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
         
        // make these private, currently public for graft exporting 
        plPlug  recipient;
        plPlug  harvest;  
          
    private:

        PLfloat    _heightOffset;
        PLfloat    _radius;
        PLfloat    _length;
        plVector3  _markDirection;
        plVector3  _markPosition;

        plMesh     _boneMesh;
        plMesh     _cartilageMesh;

        plCap      _cartilageCap;
        plCap      _boneCap;
        
        void      _setBoneColour     () const;
        void      _setCartilageColour() const;              
        void      _drawGraft() const;
                  
        void      _setCaps();        
        plCap     _findCap              ( const plModel &model );// const plSeq<plTriangle> &triangles );
        bool      _triangleIntersection ( const plTriangle &tri, plPolygon &p ) const;
        //plVector3 _pointAtAngle         ( PLfloat theta ) const;
        //PLfloat   _angleOfPoint         ( const plVector3 &v ) const;
        plVector3 _pointOnCircumference ( const plVector3 &a, const plVector3 &b ) const;

        void      _updateCartilageMesh();
        void      _updateBoneMesh     ();
        void      _updateMarkPosition ();
        
};

bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b );

#endif
