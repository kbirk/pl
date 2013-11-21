#ifndef PL_GRAFT_H
#define PL_GRAFT_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plDraw.h"
#include "plPicking.h"
#include "plVAO.h"
#include "plPlug.h"
#include "plPolygon.h"
#include "plUtility.h"
#include "plRenderer.h"
#include "plSphere.h"

class plPointAndAngle 
{
    public:
        
        PLfloat   angle;
        plVector3 point;
        
        plPointAndAngle() {}
        plPointAndAngle( const PLfloat &angle, const plVector3 &point ) 
            : angle( angle ), point( point )
        { 
        }
        
        PLbool operator < (const plPointAndAngle &p) const
        {
            return angle < p.angle;
        }
        
}; 


class plCap 
{

    public:

        std::vector<plTriangle>       triangles;
        std::vector<plPointAndAngle>  perimeter;  // perimeter vertices, ordered CCW from above
    
        plCap() {}

};


class plGraft : public plRenderable,
                public plEditable
{

    public:
   
        plGraft();      
        plGraft( const plPlug &harvest, 
                 const plPlug &recipient, 
                 PLfloat radius,
                 PLfloat cartilageThickness,
                 PLfloat heightOffset,
                 PLfloat length,                
                 const plVector3 &markDirection = plVector3(0,0,1) );

        const PLfloat   &heightOffset()       const { return _heightOffset; }
        const PLfloat   &radius()             const { return _radius; } 
        const PLfloat   &length()             const { return _length; }
        const PLfloat   &cartilageThickness() const { return _cartilageThickness; }     
        const plVector3 &markDirection()      const { return _markDirection; } 
        
        void adjustHeightOffset ( PLfloat adjustment ) { _heightOffset += adjustment; } 
        void adjustRadius       ( PLfloat adjustment ) { _radius += adjustment; if (_radius < 0) _radius = 0; _setCaps(); } 
        void adjustLength       ( PLfloat adjustment ) { _length += adjustment; if (_length < 0) _length = 0; _setCaps(); } 

        const plTransform &transform ( PLuint type ) const;
        const plPlug      &plug      ( PLuint type ) const;
        const plPlug      &harvest   () const { return _harvest;   }
        const plPlug      &recipient () const { return _recipient; }
        
        void move       ( PLuint type, const plVector3 &origin, const plVector3 &y );
        //void translate  ( PLuint type, const plVector3 &translation );
        //void translateX ( PLuint type, PLfloat distance, const plVector3 &planeNormal );
        //void translateZ ( PLuint type, PLfloat distance, const plVector3 &planeNormal );
        void rotate     ( PLuint type, const plVector3 &axis, PLfloat angleDegrees );
        //void spinMark   ( PLfloat degrees );
        void setMark    ( const plVector3 &direction ); 
         
         
        void extractRenderComponents( plRenderMap& renderMap ) const;
         
        void draw() const;

        // make these private, currently public for graft exporting 
        
          
    private:

        plPlug     _recipient;
        plPlug     _harvest;  

        PLfloat    _heightOffset;
        PLfloat    _radius;
        PLfloat    _length;
        PLfloat    _cartilageThickness;
        plVector3  _markDirection;
        plVector3  _markPosition;

        plVAO     _boneVAO;
        plVAO     _cartilageVAO;

        plCap      _boneCap;
        plCap      _cartilageCap;
                
        plVector4 _getBoneColour     () const;
        plVector4 _getCartilageColour() const;    
        
        void      _extractGraftRenderComponents( plRenderMap& renderMap ) const;
                  
        void      _drawGraft() const;
                  
        void      _setCaps();        
        void      _findCap              ( plCap &cap, const plModel &model );
        std::vector<plVector3> _pointsOutsideTriangles( plVector3 *verts, PLfloat radiusSquared ) const;
        std::vector<plVector3> _pointsInsideTriangles ( plVector3 *verts, PLfloat *dist, PLfloat radiusSquared ) const;

        bool      _triangleIntersection ( plCap &cap, const plTriangle &triangle ) const;
        plVector3 _pointOnCircumference ( const plVector3 &a, const plVector3 &b ) const;
        bool      _isBeyondHeightThresholds( const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 ) const;
        void      _generateCartilageVAO();
        void      _generateBoneVAO     ();
        void      _updateMarkPosition ();
        void      _updateCartilageThickness();
        
};


#endif
