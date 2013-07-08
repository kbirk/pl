#ifndef SPLINE_H
#define SPLINE_H

#include "pl.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plColourMesh.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plColourMap.h"
#include "plModel.h"

class plSpline : public plRenderable,
                 public plEditable
{
    public:

        plSpline();

        void draw() const;   

        void computeHermite( const plBoundary &corners, const plModel &cartilage );
           
    private:
    
        plColourMesh      _mesh;   
        plSeq<plVector3>  _s, _t;  
        
        void _drawSelectionInterface() const;
               
};


#endif

