#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include "pl.h"
#include "plError.h"
#include "plRenderable.h"
#include "plTriangle.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"

class plModel : public plRenderable
{
    public:

        plModel() {}
        plModel( std::string filename );

        bool                     isTransparent() const { return _isTransparent; }                
        const plSeq<plTriangle> &triangles()     const { return _triangles; }
        const plString          &filename()      const { return _filename; }
        
        void toggleVisibility();
        
        void draw() const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        
        plIntersection rayIntersect( const plVector3 &start, 
                                     const plVector3 &dir, 
                                     PLbool ignoreBehindRay = false, 
                                     PLbool backFaceCull = false ) const;
        /* 
        PLbool    rayIntersect( plVector3 &intPoint, 
                                plVector3 &intNorm, 
                                const plVector3 &start, 
                                const plVector3 &dir, 
                                PLbool ignoreBehindRay = false, 
                                PLbool backFaceCull = false ) const;
        */
                
	private:
	
		plMesh                _mesh;
        plSeq<plTriangle>     _triangles;
        PLbool                _isTransparent;
		plString              _filename;
};


class plBoneAndCartilage
{
    public:
       
        plBoneAndCartilage() {};
        plBoneAndCartilage( plString bone_file, plString cartilage_file);

        void readBoneFile     ( plString bone_file );
        void readCartilageFile( plString cartilage_file );

        plString getBoneFilename()      { return _bone.filename(); }
        plString getCartilageFilename() { return _cartilage.filename(); }
        
        const plSeq<plTriangle> &getBoneTriangles()      const { return _bone.triangles(); }
        const plSeq<plTriangle> &getCartilageTriangles() const { return _cartilage.triangles(); }

		void toggleBoneVisibility();
		void toggleCartilageVisibility();
		
        void draw() const;

        void getMinMax(plVector3 &min, plVector3 &max) const;   
        
        plIntersection rayIntersectBone     ( const plVector3 &start, const plVector3 &dir, 
                                              PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
                                      
		plIntersection rayIntersectCartilage( const plVector3 &start, const plVector3 &dir, 
		                                      PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
        
        /*        
        PLbool rayIntersectBone     ( plVector3 &intPoint, plVector3 &intNorm, 
                                      const plVector3 &start, const plVector3 &dir, 
                                      PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
                                      
		PLbool rayIntersectCartilage( plVector3 &intPoint, plVector3 &intNorm, 
		                              const plVector3 &start, const plVector3 &dir, 
		                              PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
		*/
		plVector3 getBoneAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        plVector3 getCartilageAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;

    private:
	
		plModel _bone;
		plModel	_cartilage;
};


#endif
