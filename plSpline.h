#ifndef SPLINE_H
#define SPLINE_H

#include "pl.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plBoundary.h"
#include "plTriangle.h"
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

        void init();
        void readFromCSV( const plSeq<plString> &row );
        void draw() const;        
        void computeHermiteSpline();
        
        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
        
    private:
    
        PLuint            _modelID;
        plColourMesh      _mesh;   
        plSeq<plVector3>  _s, _t;  
        
        void _drawCornersSelectionInterface() const;
        void _drawSplineSelectionInterface() const;
        
                
};


#endif

