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
    glDeleteTextures(1, &_tempGraftCapsID);
    glDeleteTextures(1, &_outputTextureID);

}

void plBuildDefectShader::getUniformLocations()
{
    _splineTriangleCountID = glGetUniformLocation(_shaderProgramID, "uSplineTriangleCount");   
    _siteAreaID            = glGetUniformLocation(_shaderProgramID, "uSiteArea");
}

void plBuildDefectShader::bufferGridTextures( const plSiteGrid &grid )
{
    // INPUT
    
    _siteArea = grid.area();
    
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

    glBindTexture(GL_TEXTURE_1D, 0);

    // INTERMEDIATE

    // fill graft cap texture with -1's          
    PLfloat *pixels = new PLfloat[_inputGridSize*PL_MAX_GRAFT_CAP_TRIANGLES*4*4];
          
    for (int i=0; i<_inputGridSize*PL_MAX_GRAFT_CAP_TRIANGLES*4*4; i++ )
    {
        pixels[i] = -1;
    }
               
    glGenTextures(1, &_tempGraftCapsID);                              
    glBindTexture(GL_TEXTURE_2D, _tempGraftCapsID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFT_CAP_TRIANGLES*4, _inputGridSize,  0, GL_RGBA, GL_FLOAT, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete [] pixels;

    // fill graft
    pixels = new PLfloat[_inputGridSize]; 
             
    for (int i=0; i<_inputGridSize; i++)
    {
        pixels[i] = -1;
    }
                 
    glGenTextures(1, &_tempGraftAreasID);                              
    glBindTexture(GL_TEXTURE_1D, _tempGraftAreasID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, _inputGridSize, 0, GL_R, GL_FLOAT, pixels);

    delete [] pixels;

    glBindTexture(GL_TEXTURE_1D, 0);

    // OUTPUT

    // fill output texture with -1's
    pixels = new PLfloat[_inputGridSize*4]; 
             
    for (int i=0; i<_inputGridSize*4; i++)
    {
        pixels[i] = -1;
    }

    // create empty texture for output                  
    glGenTextures(1, &_outputTextureID);                              
    glBindTexture(GL_TEXTURE_1D, _outputTextureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _inputGridSize, 0, GL_RGBA, GL_FLOAT, pixels);

    delete [] pixels;

    glBindTexture(GL_TEXTURE_1D, 0);
}

void plBuildDefectShader::bufferSplineTexture( const plSpline &spline )
{
    // set triangle count uniform
    _splineTriangleCount = spline.triangles().size();   
    
    std::cout << "tri count: " << _splineTriangleCount << "\n";
    
    // convert to 2D texture format 
    plSeq<plVector4> triangles( plVector4(0,0,0,0), _splineTriangleCount*4);
    
    for (PLuint i=0; i < _splineTriangleCount; i++)
    {
        triangles[i]                        = plVector4( spline.triangles()[i].point0(), 1);
        triangles[i+_splineTriangleCount]   = plVector4( spline.triangles()[i].point1(), 1);
        triangles[i+_splineTriangleCount*2] = plVector4( spline.triangles()[i].point2(), 1);
        triangles[i+_splineTriangleCount*3] = plVector4( spline.triangles()[i].normal(), 1);
    }
    
    // buffer triangles 
    glGenTextures(1, &_splineTextureID);
    glBindTexture(GL_TEXTURE_2D, _splineTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _splineTriangleCount, 4, 0, GL_RGBA, GL_FLOAT, &triangles[0]);

    glBindTexture(GL_TEXTURE_2D, 0);
}


PLfloat* plBuildDefectShader::dispatch()
{
    // bind compute shader
    bind();
    
    // bind input/output buffers            
    glBindImageTexture(0, _inputGridPointsID,  0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, _inputGridNormalsID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, _splineTextureID,    0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(3, _tempGraftCapsID,    0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(4, _tempGraftAreasID,   0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(5, _outputTextureID,    0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    
    // bind uniforms
    glUniform1ui(_splineTriangleCountID, _splineTriangleCount);
    glUniform1f (_siteAreaID, _siteArea);
    
    // get workgroup number
    PLuint workgroups = ceil(_inputGridSize / (PLfloat) 1024); // ensure enough workgroups are used

    // call compute shader with 1D workgrouping
    glDispatchCompute( workgroups, 1, 1 );
    
    // memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        
    std::cout << "\nCompute complete:\n";     
                
    // read output into array            
    glBindTexture(GL_TEXTURE_2D, _tempGraftCapsID);
    
    PLfloat *pixels = new PLfloat[_inputGridSize * PL_MAX_GRAFT_CAP_TRIANGLES * 4 * 4];

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);    
     
     
    
    return pixels; //new plMesh(interleaved_vertices, indices);

}

void plBuildDefectShader::loadOutputTexture()
{
    

}
        

