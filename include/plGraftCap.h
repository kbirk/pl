#pragma once

#include "plCommon.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plPicking.h"
#include "plVAO.h"
#include "plPolygon.h"
#include "plUtility.h"
#include "plTransform.h"
#include "plOctreeMesh.h"
#include "plRenderer.h"
#include "plArthroViewable.h"


#define PL_CAP_OFFSET                            plVector3(0, 0.025f, 0)
#define PL_GRAFT_DONOR_BONE_COLOUR               0.60f, 0.90f, 0.52f
#define PL_GRAFT_DONOR_CARTILAGE_COLOUR          0.30f, 0.66f, 0.22f
#define PL_GRAFT_DEFECT_BONE_COLOUR              0.90f, 0.68f, 0.27f
#define PL_GRAFT_DEFECT_CARTILAGE_COLOUR         0.80f, 0.48f, 0.07f
#define PL_GRAFT_MARKER_COLOUR                   0.80f, 0.20f, 0.80f

class plPointAndAngle
{
    public:

        PLfloat   angle;
        plVector3 point;

        plPointAndAngle() {}
        plPointAndAngle(const PLfloat &angle, const plVector3 &point)
            : angle(angle), point(point)
        {
        }

        PLbool operator < (const plPointAndAngle &p) const
        {
            return angle < p.angle;
        }

};


class plGraftCap : public plRenderable,
                   public plEditable,
                   public plArthroViewable
{
    public:

        std::vector<plTriangle>       triangles;
        std::vector<plPointAndAngle>  perimeter;  // perimeter vertices, ordered CCW from above

        plGraftCap();

        void extractRenderComponents(plRenderMap& renderMap) const;
        virtual void extractRenderComponents(plRenderMap& renderMap, PLuint technique) const;

        void generateCap(const plOctreeMesh& mesh, const plTransform& transform, PLfloat radius);

    protected:

        std::shared_ptr<plVAO> _vao;

        virtual plVector4 _getColour() const = 0;

        std::vector<plVector3> _pointsOutsideTriangles(plVector3 verts[3], const plTransform& transform, PLfloat radius) const;
        std::vector<plVector3> _pointsInsideTriangles (plVector3 verts[3], PLfloat dist[3], const plTransform& transform, PLfloat radius) const;

        bool      _triangleIntersection (const plTriangle &triangle, const plTransform& transform, PLfloat radius);
        plVector3 _pointOnCircumference (const plVector3 &a, const plVector3 &b, PLfloat radius) const;
        bool      _isBeyondHeightThresholds(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2, const plTransform& transform) const;

};


class plCartilageCap : public plGraftCap
{

    public:

        plCartilageCap();

        void generateVAO(PLfloat radius, PLfloat length, const std::vector<plPointAndAngle>& bonePerimeter);

        virtual void extractRenderComponents(plRenderMap& renderMap, PLuint technique) const;

    private:

        std::shared_ptr<plVAO> _capVAO;
        std::shared_ptr<plVAO> _projectionVAO;

        plVector4 _getColour() const;
        void _generateProjectionVAO(PLfloat radius, PLfloat length, const std::vector<plPointAndAngle>& bonePerimeter);

};


class plBoneCap : public plGraftCap
{

    public:

        plBoneCap();

        void generateVAO(PLfloat radius, PLfloat length);

    private:

        plVector4 _getColour() const;

};
