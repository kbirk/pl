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

        plSeq<plVector3> points;
        plSeq<plVector3> normals;
        
        plBoundary();
        
        plVector3 getAvgNormal() const;
        PLuint    size() const;
        void      toggleVisibility();
        
        PLuint    addPointAndNormal(const plVector3 &point, const plVector3 &normal);
        void      updateMesh(); 
        void      draw() const;     
        
    private:
    
        plMesh _mesh;    
        PLbool _showWalls;                   
};

#endif

