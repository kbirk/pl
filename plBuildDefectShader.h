#ifndef __PL_DEFECT_COMPUTE_SHADER_H__
#define __PL_DEFECT_COMPUTE_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"

#include "plMesh.h"

#define PL_DEFECT_NUM_PERTURBATIONS     4
#define PL_MAX_GRAFT_CAP_TRIANGLES      500

class plBuildDefectShader : public plShader
{
    public:                    
          
        plBuildDefectShader( const char *computeFile );
        ~plBuildDefectShader();

        void getUniformLocations();
        
        void bufferGridTextures( const plSiteGrid &grid );
        void bufferSplineTexture( const plSpline &spline );
        
        PLfloat* dispatch();        
        void loadOutputTexture();
        
    private:
    
        PLuint        _inputGridSize;
        PLuint        _inputGridPointsID; 
        PLuint        _inputGridNormalsID; 
        
        PLuint        _tempGraftCapsID;
        PLuint        _tempGraftAreasID;
        
        PLuint        _outputTextureID;

        PLuint        _splineTriangleCount;  
        PLuint        _splineTriangleCountID;       
        PLuint        _splineTextureID;
        
        PLfloat       _siteArea;
        PLuint        _siteAreaID;
               
        //PLuint        DEBUG_ID;             
        

};



#endif
