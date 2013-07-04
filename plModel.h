#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include "pl.h"
#include "plSeq.h"
#include "plError.h"
#include "plRenderable.h"
#include "plTriangle.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"

extern plSeq<plBoneAndCartilage*> _plBoneAndCartilageModels;

class plModel : public plRenderable
{
    public:

        plModel() {}
        plModel( std::string filename );

        bool                     isTransparent() const { return _isTransparent; }                
        const plSeq<plTriangle> &triangles()     const { return _triangles; }
        
        void toggleVisibility();
        
        void draw() const;

        void      getMinMax(plVector3 &min, plVector3 &max) const;        
        plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        
        plIntersection rayIntersect( const plVector3 &start, const plVector3 &dir, 
                                     PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;

        plString  getFilenameWithoutPath ();
        
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
       
        plBoneAndCartilage();
        plBoneAndCartilage( plString bone_file, plString cartilage_file );

        ~plBoneAndCartilage();

        void readFromCSV( const plSeq<plString> &row , const plString &directory );
        
        const plSeq<plTriangle> &getBoneTriangles()      const { return _bone.triangles(); }
        const plSeq<plTriangle> &getCartilageTriangles() const { return _cartilage.triangles(); }

		void toggleBoneVisibility();
		void toggleCartilageVisibility();
		
        void draw() const;

        void getMinMax(plVector3 &min, plVector3 &max) const;   
        
        plIntersection rayIntersectBone     ( const plVector3 &start, const plVector3 &dir, PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;                                     
		plIntersection rayIntersectCartilage( const plVector3 &start, const plVector3 &dir, PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;

		plVector3 getBoneAverageNormal      ( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
        plVector3 getCartilageAverageNormal ( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
    private:
	
		plModel _bone;
		plModel	_cartilage;
};


#endif
