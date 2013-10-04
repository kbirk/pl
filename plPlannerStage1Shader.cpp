#include "plPlannerStage1Shader.h"

plPlannerStage1Shader::plPlannerStage1Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


void plPlannerStage1Shader::getUniformLocations()
{
    _graftCountID          = glGetUniformLocation(_shaderProgramID, "uGraftCount");
    _graftPositionsID      = glGetUniformLocation(_shaderProgramID, "uGraftPositions");
    _graftNormalsID        = glGetUniformLocation(_shaderProgramID, "uGraftNormals");

    _defectSiteMeshSizeID  = glGetUniformLocation(_shaderProgramID, "uDefectSiteMeshSize");
                     
    _donorSiteCountID      = glGetUniformLocation(_shaderProgramID, "uDonorSiteCount");
    _donorSiteMeshSizeID   = glGetUniformLocation(_shaderProgramID, "uDonorSiteMeshSize");
    _donorSiteGridSizeID   = glGetUniformLocation(_shaderProgramID, "uDonorSiteGridSize"); 
    _donorSiteDataOffsetID = glGetUniformLocation(_shaderProgramID, "uDonorSiteDataOffset");   
}


void plPlannerStage1Shader::setGraftUniforms ( PLuint count, 
                                               const plSeq<plVector4> &positions,
                                               const plSeq<plVector4> &normals ) const
{   
    glUniform1ui  ( _graftCountID,     count );     
    glUniform4fv  ( _graftPositionsID, positions.size(), &positions[0].x );   
    glUniform4fv  ( _graftNormalsID,   normals.size(),   &normals[0].x   );
}
 
                               
void plPlannerStage1Shader::setSiteUniforms  ( PLuint defectMeshSize, 
                                               PLuint donorCount,
                                               const plSeq<PLuint> &donorMeshSize,
                                               const plSeq<PLuint> &donorGridSize,
                                               const plSeq<PLuint> &donorDataOffset) const
{

    glUniform1ui   ( _defectSiteMeshSizeID,  defectMeshSize );   
    glUniform1ui   ( _donorSiteCountID,      donorCount );   
    glUniform1uiv  ( _donorSiteMeshSizeID,   donorMeshSize.size(),   &donorMeshSize[0]   );   
    glUniform1uiv  ( _donorSiteGridSizeID,   donorGridSize.size(),   &donorGridSize[0]   );     
    glUniform1uiv  ( _donorSiteDataOffsetID, donorDataOffset.size(), &donorDataOffset[0] );
          
}

