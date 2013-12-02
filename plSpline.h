#ifndef PL_SPLINE_H
#define PL_SPLINE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plColourMap.h"
#include "plOctreeMesh.h"
#include "plVAO.h"
#include "plRenderer.h"

class plSpline : public plBoundary
{
    public:

        plSpline();
        plSpline( const plMesh &mesh );
        plSpline( const plMesh &mesh, const std::vector<plString> &row );

        const plMesh& surfaceMesh() const { return _surfaceMesh; }

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

        PLuint addPointAndNormal   ( const plVector3 &point, const plVector3 &normal );
        void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal );
        void   removePointAndNormal( PLuint index );

        void   clear();

    private:

        PLtime _lastUpdate;
        
        plMesh                   _surfaceMesh;
        std::shared_ptr< plVAO > _surfaceVAO;

        std::vector<plVector3> _averageCornerNormals() const;
              
        void _computeTangents( std::vector<PLfloat> &st, 
                               std::vector<PLfloat> &tt, 
                               const std::vector<plVector3> &p, 
                               const std::vector<plVector3> &n ) const;
                               
        void _computeHermite();

        PLuint _timeSinceLastUpdate();  

};


#endif

