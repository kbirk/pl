#include "plPlannerStage2Shader.h"

plPlannerStage2Shader::plPlannerStage2Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


void plPlannerStage2Shader::getUniformLocations()
{
    _graftCountID         = glGetUniformLocation(_shaderProgramID, "uGraftCount");
    _graftRadiiID         = glGetUniformLocation(_shaderProgramID, "uGraftRadii");

    _donorSiteCountID     = glGetUniformLocation(_shaderProgramID, "uDonorSiteCount");
    _donorSiteGridSizeID  = glGetUniformLocation(_shaderProgramID, "uDonorSiteGridSize");
        
    _seedID                = glGetUniformLocation(_shaderProgramID, "uSeed");    
}


void plPlannerStage2Shader::setGraftUniforms ( PLuint count,
                                               const plSeq<PLfloat>   &radii ) const
{   
    glUniform1ui  ( _graftCountID,     count );     
    glUniform1fv  ( _graftRadiiID,     radii.size(),     &radii[0]       );
}
 
                               
void plPlannerStage2Shader::setSiteUniforms  ( PLuint donorCount,
                                               const plSeq<PLuint> &donorGridSize) const
{ 
    glUniform1ui   ( _donorSiteCountID,     donorCount );   
    glUniform1uiv  ( _donorSiteGridSizeID,  donorGridSize.size(),  &donorGridSize[0] );   
      
    glUniform1ui  ( _seedID, rand() );       
}
