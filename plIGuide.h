#ifndef PL_I_GUIDE_H
#define PL_I_GUIDE_H

#include "plCommon.h"
#include "plModelSpecific.h"

#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"
#include "plGraft.h"
#include "plPolygon.h"
#include "plIGuideSite.h"
#include "plSpline.h"


class plPlugInfo
{

    public:
    
        const plTransform &transform          () const { return *_transform;           }
        PLfloat            radius             () const { return *_radius;              }
        PLfloat            cartilageThickness () const { return *_cartilageThickness;  }
        PLfloat            length             () const { return *_length;              }
        PLfloat            heightOffset       () const { return *_heightOffset;        }
        PLuint             type               () const { return _type;                 }
        PLuint             graftID            () const { return _graftID;              }

        plPlugInfo() {}
        plPlugInfo( const plTransform *transform, const PLfloat *radius, const PLfloat *thickness, const PLfloat *length, const PLfloat *heightOffset, PLuint type, PLuint id )
            : _transform( transform ), _radius( radius ), _cartilageThickness( thickness ), _length(length), _heightOffset(heightOffset), _type( type ), _graftID( id )
        {        
            std::cout << "type: " << type << std::endl;
        }
    
    private:
    
        const plTransform *_transform;
        const PLfloat     *_radius;
        const PLfloat     *_cartilageThickness;
        const PLfloat     *_length;
        const PLfloat     *_heightOffset;
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
        PLfloat                 toolDepth;
        PLfloat                 thickness;
        PLfloat                 printerOffset;

        std::vector<plModel*>   iGuideModelsToAdd;
        std::vector<plModel*>   iGuideModelsToSubtract;

        plIGuideSite            *site;
        PLuint                  siteID;
        
        std::vector<plPlugInfo>       plugs;
        std::vector<plKWire*>         kWires;
        std::vector<PLuint>           kWireIDs;

        std::vector<const plSpline*>  splines;
        std::vector<PLuint>           defectIDs;
        
        // MEMBERS
        // constructors
        plIGuide();
        plIGuide( plIGuideSite *s, PLuint sid, const std::vector<plPlugInfo> &p, const std::vector<plKWire*> &k, const std::vector<PLuint> &kids, const std::vector<const plSpline*> splines, std::vector<PLuint> &defectIDs );

        // core functionality
        PLbool generateIGuideModels ();
        PLbool exportIGuideModels   ( const plString &directory );
        void   clearIGuideModels    ();

        // rendering
        void extractRenderComponents( plRenderMap& renderMap ) const {};
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const {};
        
        void draw();

        void toggleVisibility();
        
    private:

        plString			_prepareFilenameWithVariables   ( PLint operation, PLchar type, PLint graftIndex, const plString &pieceName );
        std::vector<plTriangle>	_createTemplatePieceTransformed ( const std::vector<plTriangle> &baseTriObject,
                                                              const plMatrix44  &plugTransform,
                                                              const PLfloat     &zOffset,
                                                              const plVector3   &scale,
                                                              const PLfloat     &keyTranslationXAxis,
                                                              const PLfloat     &keyRotationZAxis );
      
		
    
};


#endif

