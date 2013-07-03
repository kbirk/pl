#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plRenderable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plState.h"
#include "plMesh.h"

#define PL_BOUNDARY_MESH_HEIGHT         3.0f
#define PL_BOUNDARY_MESH_WIDTH_HALF     0.25f
#define PL_BOUNDARY_MESH_CURVE_HEIGHT   0.2f

class plBoundary : public plRenderable
{
    public:

        plBoundary();       
                  
        plVector3 getAverageNormal() const;
        PLuint    size() const;
         
        const plSeq<plVector3> &points () const { return _points;  }
        const plSeq<plVector3> &normals() const { return _normals; }
        
        const plVector3        &points ( PLuint index ) const { return _points[index];  }
        const plVector3        &normals( PLuint index ) const { return _normals[index]; }
        
        void  toggleVisibility();
        
        void   loadPointAndNormal(const plString &point, const plString &normal);
        PLuint addPointAndNormal (const plVector3 &point, const plVector3 &normal);
        void   movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal);
        void   removePointAndNormal( PLuint index );
        void   updateMesh(); 
        void   draw() const;     
        //void   drawWalls() const;  
        
        friend std::ostream& operator << ( std::ostream& out, const plBoundary &b );
        
    private:
           
        plSeq<plVector3> _points;
        plSeq<plVector3> _normals;   
        plMesh           _mesh;    
        PLbool           _showWalls;                   
};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif

