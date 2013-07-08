#ifndef _PL_BOUNDARY_H
#define _PL_BOUNDARY_H

#include "pl.h"
#include "plBoundaryBase.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"

class plBoundary : public plBoundaryBase,
                   public plRenderable,
                   public plEditable
{
    public:

        plBoundary();       

        void   toggleVisibility();
        
        void   readFromCSV( const plSeq<plString> &row );

        PLuint addPointAndNormal   (const plVector3 &point, const plVector3 &normal);
        void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal);
        void   removePointAndNormal( PLuint index );
        
        void   draw() const;     
        
        friend std::ostream& operator << ( std::ostream& out, const plBoundary &b );

    private:
           
        plMesh           _mesh;    
        PLbool           _showWalls;    
        
        void             _setColour() const;    
        void             _updateMesh(); 
        
};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif

