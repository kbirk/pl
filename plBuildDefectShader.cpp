#include "plBuildDefectShader.h"

plBuildDefectShader::plBuildDefectShader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}

plBuildDefectShader::~plBuildDefectShader()
{
    glDeleteTextures(1, &_inputGridPointsID);
    glDeleteTextures(1, &_inputGridNormalsID);
    glDeleteTextures(1, &_outputTextureID);

}

void plBuildDefectShader::getUniformLocations()
{
    //_splineSizeID;
    //_splineTextureID;

}

void plBuildDefectShader::bufferGridTextures( const plSiteGrid &grid )
{
    // load input data
    _inputGridSize = grid.size();
    
    // buffer grid points 
    glGenTextures(1, &_inputGridPointsID);
    glBindTexture(GL_TEXTURE_1D, _inputGridPointsID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _inputGridSize, 0, GL_RGBA, GL_FLOAT, &grid.points(0));

    // buffer grid normals
    glGenTextures(1, &_inputGridNormalsID);
    glBindTexture(GL_TEXTURE_1D, _inputGridNormalsID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _inputGridSize, 0, GL_RGBA, GL_FLOAT, &grid.normals(0));

    // create empty texture for output                  
    glGenTextures(1, &_outputTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _outputTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _inputGridSize, PL_DEFECT_NUM_PERTURBATIONS, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void plBuildDefectShader::bufferSplineTexture( const plSpline &spline )
{
    // set triangle count
    _splineTriangleCountID = spline.triangles().size();
    
    // convert to 2D texture format 
    plSeq<plVector4> triangles( plVector4(0,0,0,0), _splineTriangleCountID*3);
    
    for (PLuint i=0; i < _splineTriangleCountID; i++)
    {
        triangles[i]                          = plVector4( spline.triangles()[i].point0(), 1);
        triangles[i+_splineTriangleCountID]   = plVector4( spline.triangles()[i].point1(), 1);
        triangles[i+_splineTriangleCountID*2] = plVector4( spline.triangles()[i].point2(), 1);
    }
    
    // buffer triangles 
    glGenTextures(1, &_splineTextureID);
    glBindTexture(GL_TEXTURE_2D, _splineTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _splineTriangleCountID, 3, 0, GL_RGBA, GL_FLOAT, &triangles[0]);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void plBuildDefectShader::dispatch()
{
    // bind compute shader
    bind();
    
    // bind input/output buffers            
    glBindImageTexture(0, _inputGridPointsID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, _inputGridNormalsID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, _splineTextureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(3, _outputTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    
    PLuint workgroups = ceil(_inputGridSize / (PLfloat) 1024); // ensure enough workgroups are used

    glDispatchCompute( workgroups, 1, 1 );
    
    // memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
                
    // read output into array            
    glBindTexture(GL_TEXTURE_2D, _outputTextureID);
    
    plVector4 *pixels = new plVector4[_inputGridSize * PL_DEFECT_NUM_PERTURBATIONS];
          
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);

    for (int i=0; i<_inputGridSize * PL_DEFECT_NUM_PERTURBATIONS; i+= _inputGridSize)
    {
        std::cout << "first: " << pixels[i] << "\n";
    }


}

void plBuildDefectShader::loadOutputTexture()
{
    

}
        

