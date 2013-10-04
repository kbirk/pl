#include "plPlannerStage2Shader.h"

plPlannerStage2Shader::plPlannerStage2Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


void plPlannerStage2Shader::getUniformLocations()
{
    _graftCountID          = glGetUniformLocation(_shaderProgramID, "uGraftCount");
    _graftPositionsID      = glGetUniformLocation(_shaderProgramID, "uGraftPositions");
    _graftNormalsID        = glGetUniformLocation(_shaderProgramID, "uGraftNormals");
    _graftRadiiID          = glGetUniformLocation(_shaderProgramID, "uGraftRadii");

    _defectSiteMeshSizeID  = glGetUniformLocation(_shaderProgramID, "uDefectSiteMeshSize");
                     
    _donorSiteCountID      = glGetUniformLocation(_shaderProgramID, "uDonorSiteCount");
    _donorSiteMeshSizeID   = glGetUniformLocation(_shaderProgramID, "uDonorSiteMeshSize");
    _donorSiteGridSizeID   = glGetUniformLocation(_shaderProgramID, "uDonorSiteGridSize");
    _donorSitePerimSizeID  = glGetUniformLocation(_shaderProgramID, "uDonorSitePerimSize");  
    _donorSiteDataOffsetID = glGetUniformLocation(_shaderProgramID, "uDonorSiteDataOffset"); 
    
    _directionID           = glGetUniformLocation(_shaderProgramID, "uDefectZ");    
}


void plPlannerStage2Shader::setGraftUniforms ( PLuint count, 
                                               const plSeq<plVector4> &positions,
                                               const plSeq<plVector4> &normals,
                                               const plSeq<PLfloat>   &radii ) const
{   
    glUniform1ui  ( _graftCountID,     count );     
    glUniform4fv  ( _graftPositionsID, positions.size(), &positions[0].x );   
    glUniform4fv  ( _graftNormalsID,   normals.size(),   &normals[0].x   );
    glUniform1fv  ( _graftRadiiID,     radii.size(),     &radii[0]       );
}
 
                               
void plPlannerStage2Shader::setSiteUniforms  ( PLuint defectMeshSize, 
                                               PLuint donorCount,
                                               const plSeq<PLuint> &donorMeshSize,
                                               const plSeq<PLuint> &donorGridSize,
                                               const plSeq<PLuint> &donorPerimSize,
                                               const plSeq<PLuint> &donorDataOffset) const
{

    glUniform1ui   ( _defectSiteMeshSizeID,  defectMeshSize );   
    glUniform1ui   ( _donorSiteCountID,      donorCount );   
    glUniform1uiv  ( _donorSiteMeshSizeID,   donorMeshSize.size(),   &donorMeshSize[0]   );   
    glUniform1uiv  ( _donorSiteGridSizeID,   donorGridSize.size(),   &donorGridSize[0]   );   
    glUniform1uiv  ( _donorSitePerimSizeID,  donorPerimSize.size(),  &donorPerimSize[0]  );   
    glUniform1uiv  ( _donorSiteDataOffsetID, donorDataOffset.size(), &donorDataOffset[0] );
          
}

void plPlannerStage2Shader::setDirectionUniform ( const plVector4 &direction ) const
{
    glUniform4fv  ( _directionID, 1, &direction.x );
}
