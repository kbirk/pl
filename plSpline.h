#ifndef SPLINE_H
#define SPLINE_H

#include "pl.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plColourMesh.h"
#include "plRenderable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plColourMap.h"

class plSpline : public plRenderable
{
    public:

        plBoundary  boundary, corners; 
                
        plSpline();

        void draw() const;
        void drawCornersSelectionInterface() const;
        void drawSplineSelectionInterface() const;
        void computeHermiteSpline();
        
        
    private:
    
        plColourMesh      _mesh;   
        plSeq<plVector3>  _s, _t;  
};

#endif

