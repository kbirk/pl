#ifndef __PL_GRAFT_BASE_H__
#define __PL_GRAFT_BASE_H__

#include "pl.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plModel.h"
#include "plTransform.h"

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


class plGraftBase 
{

    public:

        plTransform recipientTransform;        
        plTransform harvestTransform;
            
        plGraftBase();

        void init( const plSeq<plBoneAndCartilage> &models );

        void readFromCSV( const plSeq<plString> &row );

        PLfloat heightOffset()     const { return _heightOffset; } 
        PLfloat radius()           const { return _radius; } 
        PLfloat length()           const { return _length; } 
               
        PLuint  recipientModelID() const { return _recipientModelID; }
        PLuint  harvestModelID()   const { return _harvestModelID; }

        void adjustHeightOffset ( PLfloat adjustment) { _heightOffset += adjustment; } 
        virtual void adjustRadius       ( PLfloat adjustment) { _radius += adjustment; if (_radius < 0) _radius = 0; } 
        virtual void adjustLength       ( PLfloat adjustment) { _length += adjustment; if (_length < 0) _length = 0; } 

        void translateHarvest  ( const plVector3 &translation );
        void translateRecipient( const plVector3 &translation );

        void spinMark( PLfloat degrees );

        virtual void setCaps();

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
         
    protected:

        PLuint     _recipientModelID;
        PLuint     _harvestModelID;
        
        plBoneAndCartilage      *_recipientModel;
        plBoneAndCartilage      *_harvestModel;
                
        PLfloat    _heightOffset;
        PLfloat    _radius;
        PLfloat    _length;
        plVector3  _markDirection;
        plVector3  _markPosition;

        plCap      _cartilageCap;
        plCap      _boneCap;
     
        plCap     _findCap              ( const plSeq<plTriangle> &triangles );
        bool      _triangleIntersection ( const plTriangle &tri, plPoly &p ) const;
        plVector3 _pointAtAngle         ( PLfloat theta ) const;
        PLfloat   _angleOfPoint         ( const plVector3 &v ) const;
        plVector3 _pointOnCircumference ( const plVector3 &a, const plVector3 &b ) const;

        void      _updateMarkPosition();

        void      _translate( plTransform &transform, const plBoneAndCartilage &model, const plVector3 &translation );
};

bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b );

#endif
