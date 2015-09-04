#ifndef PL_OCTREE_H
#define PL_OCTREE_H

#include "plCommon.h"

#include "plVector3.h"
#include "plMath.h"
#include "plTriangle.h"
#include "plRenderable.h"
#include "plTransform.h"
#include "plVAO.h"
#include "plRenderer.h"

// children quadrant indices
// (functions rely on specific bitwise AND operations)
/*
    [0] = - - -
    [1] = + - -
    [2] = - + -
    [3] = + + -
    [4] = - - +
    [5] = + - +
    [6] = - + +
    [7] = + + +
*/


class plOctree : public plRenderable
{

    public:

        plOctree ();
        plOctree ( const plVector3 &centre, PLfloat halfWidth, PLuint depth );    // child constructor
        plOctree ( const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, PLuint depth ); // root constructor
        plOctree ( const plOctree& octree );
        plOctree ( plOctree&& octree );

        plOctree& operator= ( const plOctree& octree );
        plOctree& operator= ( plOctree&& octree );

        ~plOctree();

        PLuint depth() const { return _depth; }

        void build ( const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, PLuint depth, PLbool verbose = false );
        void clear ();

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

        PLbool rayIntersect( std::set<const plTriangle*> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLfloat rayRadius = 0.0f, PLbool ignoreBehindRay = false ) const;

        void toggleVisibility();

    private:

        PLuint                            _depth;
        plVector3                         _centre;       // center point of octree node (not strictly needed)
        PLfloat                           _halfWidth;    // half the width of the node volume (not strictly needed)
        std::vector<plOctree*>            _children;     // pointers to the eight children nodes
        std::vector< const plTriangle* >  _contained;    // triangles contained at this node

        void      _insert            ( const plTriangle &tri );
        void      _insertIntoChild   ( PLuint index, const plTriangle &tri );
        PLfloat   _sqrDistFromPoint  ( const plVector3 &point, PLint child ) const;
        plVector3 _closestPointInBox ( const plVector3 &point, PLint child ) const;
        PLbool    _sphereCheck       ( const plVector3 &centre, PLfloat radius, PLint child ) const;

        plVAO     _generateVAO( PLfloat halfWidth ) const;

        void      _move( plOctree &&octree );
        void      _copy( const plOctree& octree );

};


#endif
