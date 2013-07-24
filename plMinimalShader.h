#ifndef __PL_MINIMAL_SHADER_H__
#define __PL_MINIMAL_SHADER_H__

#include "plCommon.h"
#include "plShader.h"

class plMinimalShader : public plShader
{
    public:                    
             
        plMinimalShader(const char *vertexFile, const char *fragmentFile);
      
        virtual void getUniformLocations ();                
		virtual void setTransformUniforms( const plMatrix44 &model, const plMatrix44 &view, const plMatrix44 &projection ) const;
        virtual void setColourUniform    ( const plVector4 colour ) const;

    private:
	
        GLuint _modelID;
        GLuint _viewID;
        GLuint _projectionID; 
        GLuint _colourID; 
        

};

#endif
