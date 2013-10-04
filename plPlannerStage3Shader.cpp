#include "plPlannerStage3Shader.h"

plPlannerStage3Shader::plPlannerStage3Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


void plPlannerStage3Shader::getUniformLocations()
{
    _graftCountID          = glGetUniformLocation(_shaderProgramID, "uGraftCount");
    _graftRadiiID          = glGetUniformLocation(_shaderProgramID, "uGraftRadii");

    _totalGridPointsID     = glGetUniformLocation(_shaderProgramID, "uTotalGridPoints");
    _donorSiteCountID      = glGetUniformLocation(_shaderProgramID, "uDonorSiteCount");
    _donorSiteGridSizeID   = glGetUniformLocation(_shaderProgramID, "uDonorSiteGridSize");
    _donorSiteDataOffsetID = glGetUniformLocation(_shaderProgramID, "uDonorSiteDataOffset"); 
           
    _numDirectionsID       = glGetUniformLocation(_shaderProgramID, "uNumDirections");    
        
    _seedID                = glGetUniformLocation(_shaderProgramID, "uSeed");    
}


void plPlannerStage3Shader::setGraftUniforms ( PLuint count,
                                               PLuint numDirections,
                                               const plSeq<PLfloat>   &radii ) const
{   
    glUniform1ui  ( _graftCountID,     count );    
    glUniform1ui  ( _numDirectionsID,  numDirections );   
    glUniform1fv  ( _graftRadiiID,     radii.size(),     &radii[0]       );
}
 
                               
void plPlannerStage3Shader::setSiteUniforms  ( PLuint donorCount,
                                               PLuint totalGridPoints,
                                               const plSeq<PLuint> &donorGridSize,
                                               const plSeq<PLuint> &donorDataOffset) const
{ 
    glUniform1ui   ( _totalGridPointsID,     totalGridPoints );  
    glUniform1ui   ( _donorSiteCountID,      donorCount );   
    glUniform1uiv  ( _donorSiteGridSizeID,   donorGridSize.size(),   &donorGridSize[0] );   
    glUniform1uiv  ( _donorSiteDataOffsetID, donorDataOffset.size(), &donorDataOffset[0] );     
          
}


void plPlannerStage3Shader::setSeedUniform () const
{
    glUniform1ui  ( _seedID, rand() ); 
}
