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
    _splineTriangleCountID = glGetUniformLocation(_shaderProgramID, "uSplineTriangleCount");
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

    // create empty texture for graft candidate areas                 
    glGenTextures(1, &_candidateAreasID);                              
    glBindTexture(GL_TEXTURE_1D, _candidateAreasID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, _inputGridSize, 0, GL_R, GL_FLOAT, NULL);

    plVector4 *pixels = new plVector4[_inputGridSize * PL_DEFECT_NUM_PERTURBATIONS];
          
    for (int i=0; i<_inputGridSize * PL_DEFECT_NUM_PERTURBATIONS; i+= _inputGridSize)
    {
        pixels[i] = plVector4(-1,-1,-1,-1);
    }

    // create empty texture for output                  
    glGenTextures(1, &_outputTextureID);                              
    glBindTexture(GL_TEXTURE_2D, _outputTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _inputGridSize, PL_DEFECT_NUM_PERTURBATIONS, 0, GL_RGBA, GL_FLOAT, pixels);

    delete [] pixels;

    glBindTexture(GL_TEXTURE_2D, 0);
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


plMesh* plBuildDefectShader::dispatch()
{
    // bind compute shader
    bind();
    
    // bind input/output buffers            
    glBindImageTexture(0, _inputGridPointsID,  0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, _inputGridNormalsID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, _splineTextureID,    0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(3, _candidateAreasID,   0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
    glBindImageTexture(4, _outputTextureID,    0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    
    // bind uniforms
    glUniform1ui(_splineTriangleCountID, _splineTriangleCount);
    
    // get workgroup number
    PLuint workgroups = ceil(_inputGridSize / (PLfloat) 1024); // ensure enough workgroups are used

    glDispatchCompute( workgroups, 1, 1 );
    
    // memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
                
    // read output into array            
    glBindTexture(GL_TEXTURE_2D, _outputTextureID);
    
    plVector4 *pixels = new plVector4[_inputGridSize * PL_DEFECT_NUM_PERTURBATIONS];
          
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);

    

    std::cout << "\nCompute output:\n"; 

    plSeq<plVector3> interleaved_vertices;
    plSeq<PLuint>    indices;

    for (int i=0; i<201; i++ ) //_inputGridSize * PL_DEFECT_NUM_PERTURBATIONS; i+= _inputGridSize)
    {
        plVector4 p0 = pixels[i];
        plVector4 p1 = pixels[i+_inputGridSize];
        plVector4 p2 = pixels[i+_inputGridSize*2];
        plVector4 n = pixels[i+_inputGridSize*3];
    
        int base = interleaved_vertices.size()/2;
    
        interleaved_vertices.add( plVector3( p0.x, p0.y, p0.z ) );
        interleaved_vertices.add( plVector3( n.x, n.y, n.z ) );
        
        interleaved_vertices.add( plVector3( p1.x, p1.y, p1.z ) );
        interleaved_vertices.add( plVector3( n.x, n.y, n.z ) );
        
        interleaved_vertices.add( plVector3( p2.x, p2.y, p2.z ) );
        interleaved_vertices.add( plVector3( n.x, n.y, n.z ) );
        
        indices.add(base);
        indices.add(base+1);
        indices.add(base+2);
                        
        //std::cout << "\t" << pixels[i] << "\n";
    }

    return new plMesh(interleaved_vertices, indices);

}

void plBuildDefectShader::loadOutputTexture()
{
    

}
        

