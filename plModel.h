#ifndef __PL_MODEL_H__
#define __PL_MODEL_H__

#include "pl.h"
#include "plRenderable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plCamera.h"
#include "plModelBase.h"

class plModel : public plRenderable,
                public plModelBase
{
    public:

        plModel() : plModelBase() {}
        plModel( std::string filename );

        void toggleVisibility();

        void draw( const plVector3 &colour ) const;

        plIntersection rayIntersect( const plVector3 &start, const plVector3 &dir,
                                     PLbool ignoreBehindRay = false, PLbool backFaceCull = false ) const;
          
    private:
        plMesh                _mesh;
        PLbool                _isTransparent;
};

std::ostream& operator << ( std::ostream& out, const plModel &m );

class plBoneAndCartilage : public plBoneAndCartilageBase
{
    public:
       
        plModel bone;
		plModel	cartilage;

        plBoneAndCartilage() : plBoneAndCartilageBase() {}
        plBoneAndCartilage( plString bone_file, plString cartilage_file );

        void readFromCSV( const plSeq<plString> &row , const plString &directory );

        void draw() const;
};


#endif
