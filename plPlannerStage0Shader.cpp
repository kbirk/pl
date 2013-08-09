#include "plPlannerStage0Shader.h"

plPlannerStage0Shader::plPlannerStage0Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}

plPlannerStage0Shader::~plPlannerStage0Shader()
{
    glDeleteTextures(1, &_inputGridPointsID);
    glDeleteTextures(1, &_inputGridNormalsID);   
    glDeleteTextures(1, &_inputSiteMeshTrianglesID);           
    glDeleteTextures(1, &_outputGraftCapsID);
    glDeleteTextures(1, &_outputGraftAreasID);
}

void plPlannerStage0Shader::getUniformLocations()
{
    _inputSiteMeshSizeID = glGetUniformLocation(_shaderProgramID, "uSiteMeshSize");   
}

void plPlannerStage0Shader::bufferGridTextures( const plSiteGrid &grid )
{
    // INPUT   
    _inputSiteMeshSize = grid.triangles().size();     
    _inputGridSize     = grid.size();
    
    // buffer grid points 
    glGenTextures(1, &_inputGridPointsID);
    glBindTexture(GL_TEXTURE_1D, _inputGridPointsID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _inputGridSize, 0, GL_RGBA, GL_FLOAT, &grid.points(0));

    // buffer grid normals
    glGenTextures(1, &_inputGridNormalsID);
    glBindTexture(GL_TEXTURE_1D, _inputGridNormalsID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _inputGridSize, 0, GL_RGBA, GL_FLOAT, &grid.normals(0));

    // convert to 2D texture format 
    plSeq<plVector4> triangles( plVector4(0,0,0,0), _inputSiteMeshSize*4);
    
    for (PLuint i=0; i < _inputSiteMeshSize; i++)
    {
        triangles[i]                      = plVector4( grid.triangles()[i].point0(), 1);
        triangles[i+_inputSiteMeshSize]   = plVector4( grid.triangles()[i].point1(), 1);
        triangles[i+_inputSiteMeshSize*2] = plVector4( grid.triangles()[i].point2(), 1);
        triangles[i+_inputSiteMeshSize*3] = plVector4( grid.triangles()[i].normal(), 1);
    }
    
    // buffer triangles 
    glGenTextures(1, &_inputSiteMeshTrianglesID);
    glBindTexture(GL_TEXTURE_2D, _inputSiteMeshTrianglesID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _inputSiteMeshSize, 4, 0, GL_RGBA, GL_FLOAT, &triangles[0]);

    // OUTPUT

    // fill graft cap texture with -1's          
    plSeq<PLfloat> outputGraftCaps(-1, _inputGridSize*PL_MAX_GRAFT_CAP_TRIANGLES*4*4);   
        
    glGenTextures(1, &_outputGraftCapsID);                              
    glBindTexture(GL_TEXTURE_2D, _outputGraftCapsID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PL_MAX_GRAFT_CAP_TRIANGLES*4, _inputGridSize, 0, GL_RGBA, GL_FLOAT, &outputGraftCaps[0] );

    // fill graft area texture with -1's
    plSeq<PLfloat> outputGraftAreas(-1, _inputGridSize);   
       
    glGenTextures(1, &_outputGraftAreasID);                              
    glBindTexture(GL_TEXTURE_1D, _outputGraftAreasID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, _inputGridSize, 0, GL_R, GL_FLOAT, &outputGraftAreas[0]);

    glBindTexture(GL_TEXTURE_1D, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


PLfloat* plPlannerStage0Shader::dispatch()
{
    // bind compute shader
    bind();
    
    // bind input/output buffers            
    glBindImageTexture(0, _inputGridPointsID,  0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, _inputGridNormalsID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, _inputSiteMeshTrianglesID,  0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(3, _outputGraftCapsID,  0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(4, _outputGraftAreasID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

    // bind uniforms
    glUniform1ui(_inputSiteMeshSizeID, _inputSiteMeshSize );

    // get workgroup number
    PLuint workgroups = ceil(_inputGridSize / (PLfloat) 1024); // ensure enough workgroups are used

    // call compute shader with 1D workgrouping
    glDispatchCompute( workgroups, 1, 1 );
    
    // memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        
    std::cout << "\nAutomatic planner stage 0 complete:\n";     
                
    PLfloat *pixels = new PLfloat[_inputGridSize * PL_MAX_GRAFT_CAP_TRIANGLES * 4 * 4];
    
    // read output into array            
    glBindTexture(GL_TEXTURE_2D, _outputGraftCapsID);    
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);    

    unbind();
   
    return pixels;
}
     

