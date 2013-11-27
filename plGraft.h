#ifndef PL_GRAFT_H
#define PL_GRAFT_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plArthroViewable.h"
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
#include "plGraftCap.h"
#include "plCone.h"
#include "plCylinder.h"

class plGraft : public plRenderable,
                public plEditable,
                public plArthroViewable
{

    public:
   
        plGraft();      
        plGraft( const plPlug &harvest, 
                 const plPlug &recipient, 
                 PLfloat radius,
                 PLfloat length,                
                 const plVector3 &markDirection = plVector3(0,0,1) );

        const PLfloat&     radius()             const { return _radius; } 
        const PLfloat&     length()             const { return _length; }  
        const plVector3&   markDirection()      const { return _markDirection; } 
        const plVector3&   markPositions( PLuint index ) const { return _markPositions[ index ]; } 
        const plTransform& transform ( PLuint type ) const;
        const plVector3&   surfaceNormal( PLuint type ) const;
        const plPlug&      plug      ( PLuint type ) const;
        const plPlug&      harvest   () const { return _harvest;   }
        const plPlug&      recipient () const { return _recipient; }

        void adjustRadius       ( PLfloat adjustment ) { _radius += adjustment; if (_radius < 0) _radius = 0; _generateCaps(); } 
        void adjustLength       ( PLfloat adjustment ) { _length += adjustment; if (_length < 0) _length = 0; _generateCaps(); } 

        void move       ( PLuint type, const plVector3& origin, const plVector3& y, const plVector3& surfaceNormal );
        //void rotate     ( PLuint type, const plVector3 &axis, PLfloat angleDegrees );
        void setMark    ( const plVector3 &direction ); 
             
        void toggleArthroView() { plArthroViewable::toggleArthroView(); _cartilageCap.toggleArthroView(); }    
                            
        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

    private:

        plPlug     _recipient;
        plPlug     _harvest;

        PLfloat    _radius;
        PLfloat    _length;

        plVector3  _markDirection;
        plVector3  _markPositions[4];

        plBoneCap      _boneCap;
        plCartilageCap _cartilageCap;
                      
        void  _extractGraftRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

        void  _generateCaps();     
        void  _updateMarkPosition();


};


#endif
