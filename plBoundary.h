#ifndef PL_BOUNDARY_H
#define PL_BOUNDARY_H

#include "plCommon.h"
#include "plMath.h"

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
        plBoundary( const std::vector<plString> &row );
 
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

        std::vector<plVector3> _points;       // always in counterclockwise direction
        std::vector<plVector3> _normals;   
        plMesh           _mesh;    

        void             _setColour() const;    
        void             _drawPoints() const;
        void             _updateMesh(); 
        
};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif

