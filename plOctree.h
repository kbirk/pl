#ifndef PL_OCTREE_H
#define PL_OCTREE_H

#include "plCommon.h"

#include "plVector3.h"
#include "plMath.h"
#include "plLineMesh.h"
#include "plTriangle.h"
#include "plRenderable.h"
#include "plTransform.h"

// NOTE:    plOctrees are not exclusive, if a triangle is in a node, it will also be in its parent node
//          as well. This is not as memory efficient, but gives much better performance for queries

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

        void insert( const plTriangle &tri, PLuint depth);
        void clear ();        
        void draw() const;        

        PLbool rayIntersect( plSet<const plTriangle*> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLfloat boxInflation = 0, PLbool ignoreBehindRay = false ) const;
               
    private:
                         
        plLineMesh _mesh;

        plVector3  _centre;                             // center point of octree node (not strictly needed)    
        PLfloat    _halfWidth;                          // half the width of the node volume (not strictly needed)           
        std::vector< const plTriangle* >  _contained;   // triangles contained at this node 
        std::vector< plOctreeNode* >      _children;    // pointers to the eight children nodes   
        
        void      _insertChild( PLuint index, const plTriangle &tri, PLuint depth);
        PLfloat   _squaredDistanceFromPoint( const plVector3 &point, PLint child = -1 ) const;
        plVector3 _closestPointInBox( const plVector3 &point, PLint child ) const;
        PLbool    _sphereCheck      ( const plVector3 &centre, PLfloat radius, PLint child = -1 ) const;        
        void      _updateMesh();
        
};


class plOctree : public plRenderable
{
    public:
          
        plOctree();        
        plOctree( const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, PLuint depth );

        ~plOctree();
         
        void build( const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, PLuint depth);  
              
        plIntersection rayIntersect ( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay = false,  PLbool backFaceCull = false ) const;
        
        void graftIntersect ( plSet<const plTriangle*> &triangles, const plTransform &transform, PLfloat radius) const;

        void draw() const;
        
    private:
           
        plOctreeNode *_root;   
          
        void _clear();       
        void _fill ( const std::vector<plTriangle> &triangles, PLuint depth );
           
        // prevent copy construction and assignment   
        plOctree( const plOctree &o );
        plOctree operator= ( const plOctree &o ) const;    
    
        
};

#endif
