#include "plPlannerStage1Shader.h"

plPlannerStage1Shader::plPlannerStage1Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


void plPlannerStage1Shader::getUniformLocations()
{
    _graftCountID         = glGetUniformLocation(_shaderProgramID, "uGraftCount");
    _graftPositionsID     = glGetUniformLocation(_shaderProgramID, "uGraftPositions");
    _graftNormalsID       = glGetUniformLocation(_shaderProgramID, "uGraftNormals");
    _graftRadiiID         = glGetUniformLocation(_shaderProgramID, "uGraftRadii");

    _defectSiteMeshSizeID = glGetUniformLocation(_shaderProgramID, "uDefectSiteMeshSize");
                     
    _donorSiteCountID     = glGetUniformLocation(_shaderProgramID, "uDonorSiteCount");
    _donorSiteMeshSizeID  = glGetUniformLocation(_shaderProgramID, "uDonorSiteMeshSize");
    _donorSiteGridSizeID  = glGetUniformLocation(_shaderProgramID, "uDonorSiteGridSize");
    _donorSitePerimSizeID = glGetUniformLocation(_shaderProgramID, "uDonorSitePerimSize");    
}


void plPlannerStage1Shader::setGraftUniforms ( PLuint count, 
                                               const plSeq<plVector4> &positions,
                                               const plSeq<plVector4> &normals,
                                               const plSeq<PLfloat>   &radii ) const
{   
    glUniform1ui  ( _graftCountID, count );     
    glUniform4fv  ( _graftPositionsID, positions.size(), &positions[0].x );   
    glUniform4fv  ( _graftNormalsID,   normals.size(),   &normals[0].x   );
    glUniform4fv  ( _graftRadiiID,     radii.size(),     &radii[0]       );
}
                               
void plPlannerStage1Shader::setSiteUniforms  ( PLuint defectMeshSize, 
                                               PLuint donorCount,
                                               PLuint donorMeshSize,
                                               PLuint donorGridSize,
                                               PLuint donorPerimSize) const
{

    glUniform1ui  ( _defectSiteMeshSizeID, defectMeshSize );   
    glUniform1ui  ( _donorSiteCountID,     donorCount );   
    glUniform1ui  ( _donorSiteMeshSizeID,  donorMeshSize );   
    glUniform1ui  ( _donorSiteGridSizeID,  donorGridSize );   
    glUniform1ui  ( _donorSitePerimSizeID, donorPerimSize );   
    
}
