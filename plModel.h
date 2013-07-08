#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include "pl.h"
#include "plSeq.h"
#include "plRenderable.h"
#include "plTriangle.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"
#include "plCamera.h"

class plModel : public plRenderable
{
    public:

        plModel() {}
        plModel( std::string filename );
             
        const plSeq<plTriangle> &triangles()     const { return _triangles; }
        
        void toggleVisibility();
        
        void draw( const plVector3 &colour ) const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        plString  getFilenameWithoutPath ();

        plIntersection rayIntersect( const plVector3 &start, const plVector3 &dir, 
                                     PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;

        friend std::ostream& operator << ( std::ostream& out, const plModel &m );
          
	private:
	
		plMesh                _mesh;
        plSeq<plTriangle>     _triangles;
        PLbool                _isTransparent;
		plString              _filename;
};

std::ostream& operator << ( std::ostream& out, const plModel &m );

class plBoneAndCartilage
{
    public:
       
        plModel bone;
		plModel	cartilage;

        plBoneAndCartilage();
        plBoneAndCartilage( plString bone_file, plString cartilage_file );

        void readFromCSV( const plSeq<plString> &row , const plString &directory );

        void draw() const;

        void getMinMax(plVector3 &min, plVector3 &max) const;   

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
};


#endif
