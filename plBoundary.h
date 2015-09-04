#ifndef PL_BOUNDARY_H
#define PL_BOUNDARY_H

#include "plCommon.h"
#include "plMath.h"

#include "plVector3.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPicking.h"
#include "plMeshSpecific.h"
#include "plVAO.h"
#include "plRenderer.h"

#define PL_BOUNDARY_SELECTED_POINT_RADIUS        1.0f
#define PL_BOUNDARY_POINT_RADIUS                 0.75f
#define PL_BOUNDARY_MESH_HEIGHT                  1.0f
#define PL_BOUNDARY_MESH_WIDTH_HALF              0.4f
#define PL_BOUNDARY_MESH_CURVE_HEIGHT            0.2f

#define PL_BOUNDARY_SELECTED_COLOUR              0.20f, 0.90f, 0.70f
#define PL_BOUNDARY_POINT_SELECTED_COLOUR        0.20f, 0.90f, 0.70f

#define PL_BOUNDARY_DEFECT_CORNER_COLOUR         0.90f, 0.20f, 0.20f
#define PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR       0.55f, 0.21f, 0.55f
#define PL_BOUNDARY_DONOR_COLOUR                 0.40f, 0.40f, 0.80f
#define PL_BOUNDARY_IGUIDE_COLOUR                0.20f, 0.53f, 0.45f

class plBoundary : public plRenderable,
                   public plEditable,
                   public plMeshSpecific
{
    public:

        plBoundary();
        plBoundary( PLuint type, const plMesh& mesh );
        plBoundary( PLuint type, const plMesh& mesh, const std::vector<plString> &row );

        PLuint size() const { return _points.size(); }
        PLuint type() const { return _type; }

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

        const plVector3& points ( PLuint index ) const { return _points[index];  }
        const plVector3& normals( PLuint index ) const { return _normals[index]; }

        plVector3 getAverageNormal() const;
        plVector3 getCentroid() const;

        virtual PLuint addPointAndNormal   ( const plVector3 &point, const plVector3 &normal);
        virtual void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal);
        virtual void   removePointAndNormal( PLuint index );
        virtual void   clear();

    protected:

        PLint _type;

        std::vector< plVector3 > _points;       // always in counterclockwise direction
        std::vector< plVector3 > _normals;

        std::shared_ptr< plVAO > _vao;

        plVector4 _getColour() const;

        void _generateVAO();
        void _extractPointRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif
