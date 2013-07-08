#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"


class plBoundary : public plRenderable,
                   public plEditable
{
    public:

        plBoundary();       
          
        PLuint size() const;

        const plSeq<plVector3> &points () const { return _points;  } 
        const plSeq<plVector3> &normals() const { return _normals; }
        
        const plVector3        &points ( PLuint index ) const { return _points[index];  }
        const plVector3        &normals( PLuint index ) const { return _normals[index]; }

        plVector3 getAverageNormal() const;

        void   toggleVisibility();
        
        void   readFromCSV( const plSeq<plString> &row );

        PLuint addPointAndNormal (const plVector3 &point, const plVector3 &normal);
        void   movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal);
        void   removePointAndNormal( PLuint index );
        
        void   draw() const;     
        
        friend std::ostream& operator << ( std::ostream& out, const plBoundary &b );

    private:
           
        plSeq<plVector3> _points;       // always in counterclockwise direction
        plSeq<plVector3> _normals;   
        plMesh           _mesh;    
        PLbool           _showWalls;    
        
        void             _setColour() const;    
        void             _updateMesh(); 
        
};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif

