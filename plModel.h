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
        
        PLbool operator < (const plOrderPair &o) const
        {
            return distance < o.distance;
        }
    
};

class plModel : public plRenderable
{
    public:

        plModel() {}
        plModel( std::string filename );
             
        const plSeq<plTriangle> &triangles() const { return _triangles; }
        const plOctree          &octree()    const { return _octree;    }
        plString                 filename()  const { return _filename;  }  
        
        void toggleVisibility();        
        void draw( const plVector3 &colour ) const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        
        plIntersection rayIntersect( const plVector3 &start, const plVector3 &dir, 
                                     PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;

        friend std::ostream& operator << ( std::ostream& out, const plModel &m );
              
	private:
	
		plMesh                _mesh;
        plSeq<plTriangle>     _triangles;
        plOctree              _octree;
        PLbool                _isTransparent;
		plString              _filename;

};

std::ostream& operator << ( std::ostream& out, const plModel &m );


#endif
