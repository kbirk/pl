#ifndef __PL_I_GUIDE_H__ 
#define __PL_I_GUIDE_H__

#include "plCommon.h"
#include "plModelSpecific.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"
#include "plGraft.h"
#include "plPolygon.h"
#include "plIGuideSite.h"


class plPlugInfo
{

    public:
    
        const plTransform &transform() const { return *_transform; } 
        PLfloat            radius   () const { return *_radius;    }
        PLuint             type     () const { return _type;       }
        PLuint             graftID  () const { return _graftID;    }

        plPlugInfo() {}
        plPlugInfo( const plTransform *transform, const PLfloat *radius, PLuint type, PLuint id ) 
            : _transform( transform ), _radius( radius ), _type( type ), _graftID( id )
        {        
        }
    
    private:
    
        const plTransform *_transform;
        const PLfloat     *_radius;
        PLuint             _type;
        PLuint             _graftID;    
        

};


class plKWire 
{
    public:

        PLfloat   thickness;
        plVector3 position;
        plVector3 direction;
  
        plKWire() {}
};


class plIGuide : public plRenderable
{

    public:

        // ATTRIBUTES
        // geometry data
        PLfloat                 thickness;
        PLfloat                 printerOffset;

        plSeq<plModel*>         iGuideModelsToAdd;
        plSeq<plModel*>         iGuideModelsToSubtract;

        plIGuideSite           *site;
        PLuint                  siteID;
        
        plSeq<plPlugInfo>       plugs;
        plSeq<plKWire*>         kWires;
        plSeq<PLuint>           kWireIDs;
        
        // MEMBERS
        // constructors
        plIGuide();
        plIGuide( plIGuideSite *s, PLuint sid, const plSeq<plPlugInfo> &p, const plSeq<plKWire*> &k, const plSeq<PLuint> &kids );

        // core functionality
        PLbool generateIGuideModels ();
        PLbool exportIGuideModels   ( const plString &directory );
        void   clearIGuideModels    ();

        // rendering
        void draw();

        void toggleVisibility();
        
    private:

	    plString			_prepareFilenameWithVariables   ( PLbool add, PLchar type, PLint graftIndex, const plString &pieceName );
        plSeq<plTriangle>	_createTemplatePieceTransformed ( const plSeq<plTriangle> &baseTriObject,
                                                              const plMatrix44  &plugTransform,
                                                              const PLdouble    &zOffset,
                                                              const plVector3   &scale,
                                                              const PLdouble    &keyTranslationXAxis,
                                                              const PLdouble    &keyRotationZAxis );
      
		
    
};


#endif

