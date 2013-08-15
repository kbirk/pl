#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plMesh.h"
#include "plOctree.h"
#include "plTriangle.h"
#include "plRenderable.h"
#include "plPickingTexture.h"
#include "plRenderingPipeline.h"

// used to order triangles for drawing transparent meshes
class plOrderPair
{
    public:
    
        int   index;
        float distance;

        plOrderPair (int i, float d)
            : index(i), distance(d)
        {
        }
    
};

class plModel : public plRenderable
{
    public:
        
        plModel( std::string filename, PLuint octreeDepth = 7 );
             
        const plSeq<plTriangle> &triangles() const { return _triangles; }
        const plOctree          &octree()    const { return _octree;    }
        plString                 filename()  const { return _filename;  }
        
        void toggleVisibility();
        void toggleTransparency();
        const PLbool isTransparent() const { return _isTransparent; }

        void draw( const plVector3 &colour ) const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
                
        plIntersection rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
     
	private:
	
		plMesh                _mesh;
        plSeq<plTriangle>     _triangles;
        plOctree              _octree;
        PLbool                _isTransparent;
		plString              _filename;

        // prevent empty constructor, copy constructor, and assignment, which will invalidate the octree's pointers if rhs is scoped
        plModel();
        plModel( const plModel &m );
        plModel operator=(const plModel &m) const;

};

std::ostream& operator << ( std::ostream& out, const plModel &m );


#endif
