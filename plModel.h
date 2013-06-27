#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include <algorithm>
#include "pl.h"
#include "plError.h"
#include "plRenderable.h"
#include "plTriangle.h"
#include "plCamera.h"
#include "plGraft.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"

class plModel : public plRenderable
{
    public:

        plModel() {}
        plModel( std::string filename );

        ~plModel()

        bool isTransparent() const                    { return _isTransparent; }                
        const plSeq<plTriangle> &getTriangles() const { return _triangles; }
        
        void toggleVisibility();
        
        void draw() const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up );
        PLbool    rayIntersect( plVector3 &intPoint, plVector3 &intNorm, 
                                const plVector3 &start, const plVector3 &dir, 
                                PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
        
        
	private:
	
		plMesh                _mesh;
        plSeq<plTriangle>     _triangles;
        PLbool                _isTransparent;
		
};


class plBoneAndCartilage
{
    public:
       
        plBoneAndCartilage() {};
        plBoneAndCartilage( plString bone_file, plString cartilage_file);

        const plSeq<plTriangle> &getBoneTriangles()      const { return _bone.getTriangles(); }
        const plSeq<plTriangle> &getCartilageTriangles() const { return _cartilage.getTriangles(); }

		void toggleBoneVisibility();
		void toggleCartilageVisibility();
		
        void draw() const;

        void   getMinMax(plVector3 &min, plVector3 &max) const;           
        PLbool rayIntersectBone     ( plVector3 &intPoint, plVector3 &intNorm, 
                                      const plVector3 &start, const plVector3 &dir, 
                                      PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
		PLbool rayIntersectCartilage( plVector3 &intPoint, plVector3 &intNorm, 
		                              const plVector3 &start, const plVector3 &dir, 
		                              PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
		
		plVector3 getBoneAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up );
        plVector3 getCartilageAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up );

    private:
	
		plModel _bone;
		plModel	_cartilage;
};


#endif
