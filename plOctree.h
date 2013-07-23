#ifndef __PL_OCTREE_H__
#define __PL_OCTREE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plMath.h"
#include "plLineMesh.h"
#include "plTriangle.h"
#include "plRenderable.h"
#include "plTransform.h"

// NOTE:    plOctrees are not exclusive, if a triangle is in a node, it will also be in its parent node
//          as well. This is not as memory efficient, but gives much better performance

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

class plOctreeNode 
{
    public:

        plOctreeNode( const plVector3 &c, PLfloat hw ); 

        ~plOctreeNode();

        plVector3                 centre;        // center point of octree node (not strictly needed)    
        PLfloat                   halfWidth;     // half the width of the node volume (not strictly needed)    
        plSeq<plOctreeNode*>      children;      // pointers to the eight children nodes   
        plSeq<const plTriangle*>  contained;     // linked list of objects contained at this node 
        
        void    draw  () const;        
        void    insert( const plTriangle &tri, PLuint depth);
        
        PLfloat squaredDistanceFromPoint( const plVector3 &point, PLint child = -1 ) const;
        PLbool  sphereCheck             ( const plVector3 &centre, PLfloat radius, PLint child = -1 ) const;
        
        PLbool rayIntersect   ( plSet<const plTriangle*> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLfloat boxInflation = 0, PLbool ignoreBehindRay = false ) const;
        
        
    private:
                           
        plLineMesh  _mesh;

        void _insertChild( PLuint index, const plTriangle &tri, PLuint depth);
        void _updateMesh();
        
};

class plOctree : public plRenderable
{
    public:
          
        plOctree();        
        plOctree( const plVector3 &min, const plVector3 &max, const plSeq<plTriangle> &triangles, PLuint depth );

        ~plOctree();

        void build( const plVector3 &min, const plVector3 &max, const plSeq<plTriangle> &triangles, PLuint depth); 

        plIntersection rayIntersect ( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay = false,  PLbool backFaceCull = false ) const;
        
        void graftIntersect ( plSet<const plTriangle*> &triangles, const plTransform &transform, PLfloat radius) const;

        void draw() const;
        
    private:
           
        plOctree( const plOctree &o );
        plOctree operator= ( const plOctree &o ) const;    
    
        plOctreeNode *_root;
    
        void _fill(const plSeq<plTriangle> &triangles, PLuint depth);

};

#endif
