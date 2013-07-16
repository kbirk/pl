#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plRenderable.h"
#include "plTriangle.h"
#include "plPickingTexture.h"
#include "plMesh.h"
#include "plRenderingPipeline.h"
#include "plOctree.h"

class plModel : public plRenderable
{
    public:

        plModel() {}
        plModel( std::string filename );
             
        const plSeq<plTriangle> &triangles() const { return _triangles; }
        plString                 filename()  const { return _filename;  }  
        
        void toggleVisibility();        
        void draw( const plVector3 &colour ) const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        
        plIntersection rayIntersect( const plVector3 &start, const plVector3 &dir, 
                                     PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;

        friend std::ostream& operator << ( std::ostream& out, const plModel &m );
         
         plOctree              _octree;
          
	private:
	
		plMesh                _mesh;
        plSeq<plTriangle>     _triangles;
        
        PLbool                _isTransparent;
		plString              _filename;

};

std::ostream& operator << ( std::ostream& out, const plModel &m );


#endif
