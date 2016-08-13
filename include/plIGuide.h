#ifndef PL_I_GUIDE_H
#define PL_I_GUIDE_H

#include "plCommon.h"
#include "plMeshSpecific.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"
#include "plGraft.h"
#include "plPolygon.h"
#include "plIGuideSite.h"
#include "plSpline.h"


#define PL_TOOL_DEPTH                      43.5f
#define PL_BASE_TRANSLATION_DISTANCE       1.01f
#define PL_BASE_THICKNESS                  5.00f
#define PL_INTERIOR_TRANSLATION_DISTANCE  -10.00
#define PL_INTERIOR_THICKNESS              11.0f


class plPlugInfo
{

    public:

        plTransform transform() const { return _plug->finalTransform(); }
        PLfloat     radius   () const { return *_radius; }
        PLfloat     length   () const { return *_length; }
        PLuint      type     () const { return _type;    }
        PLuint      graftID  () const { return _graftID; }

        plPlugInfo() {}
        plPlugInfo( const plPlug  *plug,
                    const PLfloat *radius,
                    const PLfloat *length,
                    PLuint type,
                    PLuint id )
            : _plug( plug ), _radius( radius ), _length(length), _type( type ), _graftID( id )
        {
        }

    private:

        const plPlug*  _plug;
        const PLfloat* _radius;
        const PLfloat* _length;
        PLuint         _type;
        PLuint         _graftID;
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

        plIGuideSite *site;
        PLuint        siteID;

        std::vector<plPlugInfo>      plugs;
        std::vector<plKWire*>        kWires;
        std::vector<PLuint>          kWireIDs;

        std::vector<const plSpline*> splines;
        std::vector<PLuint>          defectIDs;

        plIGuide();
        plIGuide( plIGuideSite *site,
                  PLuint siteID,
                  const std::vector<plPlugInfo>&      plugs,
                  const std::vector<plKWire*>&        kwires,
                  const std::vector<PLuint>&          kwireIDs,
                  const std::vector<const plSpline*>& splines,
                  std::vector<PLuint>&                defectIDs );

        virtual ~plIGuide();

        PLbool generateIGuideModels();
        PLbool exportIGuideModels  ( const std::string &directory );
        void   clearIGuideModels   ();

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

    private:

        std::vector<plModel*> _modelsToAdd;
        std::vector<plModel*> _modelsToSubtract;

        std::string	_generateOutputName( PLint operation, PLchar type, PLint graftIndex, const std::string &pieceName );

        std::vector<plTriangle>	_transformTemplate ( const std::vector<plTriangle> &baseTriObject,
                                                     const plMatrix44  &plugTransform,
                                                     const PLfloat     &zOffset,
                                                     const plVector3   &scale,
                                                     const PLfloat     &keyTranslationXAxis,
                                                     const PLfloat     &keyRotationZAxis );

        std::vector<plTriangle> _translateTriangles( const std::vector< plTriangle >& triangles, const plVector3& translation );


};


#endif
