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
#include "plGraftCap.h"


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

        const PLfloat&     heightOffset()       const { return _heightOffset; }
        const PLfloat&     radius()             const { return _radius; } 
        const PLfloat&     length()             const { return _length; }
        const PLfloat&     cartilageThickness() const { return _cartilageThickness; }     
        const plVector3&   markDirection()      const { return _markDirection; } 
        const plTransform& transform ( PLuint type ) const;
        const plPlug&      plug      ( PLuint type ) const;
        const plPlug&      harvest   () const { return _harvest;   }
        const plPlug&      recipient () const { return _recipient; }

        void adjustHeightOffset ( PLfloat adjustment ) { _heightOffset += adjustment; } 
        void adjustRadius       ( PLfloat adjustment ) { _radius += adjustment; if (_radius < 0) _radius = 0; _generateCaps(); } 
        void adjustLength       ( PLfloat adjustment ) { _length += adjustment; if (_length < 0) _length = 0; _generateCaps(); } 

        void move       ( PLuint type, const plVector3 &origin, const plVector3 &y );
        void rotate     ( PLuint type, const plVector3 &axis, PLfloat angleDegrees );
        void setMark    ( const plVector3 &direction ); 
                 
        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractEditorRenderComponents( plRenderMap& renderMap ) const;

    private:

        plPlug     _recipient;
        plPlug     _harvest;  

        PLfloat    _heightOffset;
        PLfloat    _radius;
        PLfloat    _length;
        PLfloat    _cartilageThickness;
        plVector3  _markDirection;
        plVector3  _markPosition;

        plBoneCap      _boneCap;
        plCartilageCap _cartilageCap;
                      
        void  _extractGraftRenderComponents( plRenderMap& renderMap ) const;
        void  _extractGraftEditorRenderComponents( plRenderMap& renderMap ) const;

        void  _generateCaps();     
        void  _updateMarkPosition();
        void  _updateCartilageThickness();

};


#endif
