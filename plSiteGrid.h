#ifndef _PL_SITE_GRID_H_
#define _PL_SITE_GRID_H_

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"
#include "plTriangle.h"
#include "plBoundary.h"
#include "plMeshCutter.h"

class plSiteGrid
{
    public:
    
        plSiteGrid();
        plSiteGrid( const plSeq<plTriangle> &triangles, const plBoundary &boundary );
        
        PLuint     size()              const { return _points.size(); }
        PLfloat    area()              const { return _area; } 
        plVector3  normal()            const { return _normal; };
        
        const plTriangle&        triangles(PLuint i)       const { return _triangles[i];   }        
        const plSeq<plTriangle>& triangles()               const { return _triangles;      }
        const plVector4&         points   ( PLuint index ) const { return _points[index];  }
        const plVector4&         normals  ( PLuint index ) const { return _normals[index]; }

            
    private:
        
        plSeq<plTriangle> _triangles;
        plSeq<plVector4>  _points; 
        plSeq<plVector4>  _normals;
        PLfloat           _area;
        plVector3         _normal;
        
        void _generateGridPoints();  
        void _calcArea          ();
        void _calcNormal        ();
};

#endif
