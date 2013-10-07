#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "plCommon.h"
#include "plMath.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPickingTexture.h"
#include "plBoneAndCartilage.h"
#include "plMesh.h"
#include "plDraw.h"

class plBoundary : public plRenderable,
                   public plEditable
{
    public:

        plBoundary();      
        plBoundary( const plSeq<plString> &row );
 
        PLuint size() const;

        const plVector3        &points ( PLuint index ) const { return _points[index];  }
        const plVector3        &normals( PLuint index ) const { return _normals[index]; }

        plVector3 getAverageNormal() const;

        virtual PLuint addPointAndNormal   ( const plVector3 &point, const plVector3 &normal);
        virtual void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal);
        virtual void   removePointAndNormal( PLuint index );
        virtual void   clear();        
        virtual void   draw() const;     
        
    protected:

        plSeq<plVector3> _points;       // always in counterclockwise direction
        plSeq<plVector3> _normals;   
        plMesh           _mesh;    

        void             _setColour() const;    
        void             _drawPoints() const;
        void             _updateMesh(); 
        
};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif

