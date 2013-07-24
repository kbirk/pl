#ifndef __PL_DEFECT_COMPUTE_SHADER_H__
#define __PL_DEFECT_COMPUTE_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"

#define PL_DEFECT_NUM_PERTURBATIONS     4

class plBuildDefectShader : public plShader
{
    public:                    
          
        plBuildDefectShader( const char *computeFile );
        ~plBuildDefectShader();

        void getUniformLocations();
        
        void bufferGridTextures( const plSiteGrid &grid );
        void bufferSplineTexture( const plSpline &spline );
        
        void dispatch();        
        void loadOutputTexture();
        
    private:
    
        PLuint        _inputGridSize;
        PLuint        _inputGridPointsID; 
        PLuint        _inputGridNormalsID; 
        
        PLuint        _outputTextureID;
        //PLuint        _outputTextureY;
        
        PLuint        _splineTriangleCountID;
        PLuint        _splineTextureID;
               
                      
        

};



#endif
