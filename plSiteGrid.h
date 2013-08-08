#ifndef _PL_SITE_GRID_H_
#define _PL_SITE_GRID_H_

#include "plCommon.h"
#include "plSeq.h"
#include "plVector4.h"
#include "plTriangle.h"

class plSiteGrid
{
    public:
    
        plSiteGrid();
        
        PLuint  size() const { return _points.size(); }
        PLfloat area() const { return _area; } 
        
        void clear();
        
        const plVector4& points( PLuint index )  const  { return _points[index];  }
        const plVector4& normals( PLuint index ) const  { return _normals[index]; }
        
        void addPointAndNormal( const plVector4 &p, const plVector4 &n );
           
        void calcArea( const plSeq<plTriangle> &triangles );
            
    private:
    
        plSeq<plVector4>  _points; 
        plSeq<plVector4>  _normals;
        PLfloat           _area;
        
};

#endif
