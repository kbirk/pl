#ifndef PL_PLANNING_SITE_H
#define PL_PLANNING_SITE_H

#include "plCommon.h"

#include "plVector4.h"
#include "plTriangle.h"
#include "plBoundary.h"
#include "plMeshCutter.h"
#include "plSSBO.h"

class plPointAndNormal
{

    public:
    
        plVector3 point;
        plVector3 normal;
    
        plPointAndNormal( const plVector3 &p, const plVector3 &n)
            : point(p), normal(n)
        {}

        PLbool operator < (const plPointAndNormal &pn) const
        {       
            return point.x < pn.point.x || point.y < pn.point.y || point.z < pn.point.z;
        }
};


class plPlanningSite
{
    public:
    
        std::vector<plTriangle> triangles;
        std::vector<plVector4>  gridPoints; 
        std::vector<plVector4>  gridNormals;
        std::vector<plVector4>  boundaryPoints;
        std::vector<plVector4>  boundaryNormals;
        PLfloat           area;
        plVector3         avgNormal;
    
        plPlanningSite();
        plPlanningSite( const std::vector<plTriangle> &triangles, const plBoundary &boundary, PLbool fineGrain = false );          
        plPlanningSite( plPlanningSite&& site );

        plPlanningSite& operator= ( plPlanningSite&& site ); 

        PLuint totalSize() const { return gridPoints.size() + gridNormals.size() + triangles.size()*4 + boundaryPoints.size() + boundaryNormals.size(); }
        
        void   getData( std::vector<plVector4> &data ) const { _bufferGridData( data ); _bufferMeshData( data ); _bufferBoundaryData( data ); }
        plSSBO getSSBO() const;
        
        plVector3 getSmoothNormal( const plVector3 &centroid, const plVector3 &up, PLfloat radius ) const;
        
        PLbool good() const;

    private:

        void _bufferGridData     ( std::vector<plVector4> &data ) const;
        void _bufferMeshData     ( std::vector<plVector4> &data ) const;
        void _bufferBoundaryData ( std::vector<plVector4> &data ) const;
        
        
        void _generateCoarseGridPoints ();  
        void _generateFineGridPoints   ();  
        void _generateBoundaryPoints   ( const plBoundary &boundary );
        void _calcArea   ();
        void _calcNormal ();

};


#endif
