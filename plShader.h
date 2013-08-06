#ifndef __PL_SHADER_H__
#define __PL_SHADER_H__

#include "plCommon.h"
#include "plMatrix44.h"

// attribute locations, set by VBOs and mirrored in shader files
#define PL_POSITION_ATTRIBUTE  0
#define PL_NORMAL_ATTRIBUTE    1
#define PL_COLOUR_ATTRIBUTE    2
#define PL_TEXCOORD_ATTRIBUTE  3

class plShader 
{

    public:                    
          
        plShader(const char *shaderFile, GLenum shaderType);           
        plShader(const char *vertexFile, const char *fragmentFile);

		~plShader();
                
		void bind()   const { glUseProgram(_shaderProgramID);  }	    
		void unbind() const { glUseProgram(0);                 }
		
		virtual void getUniformLocations() = 0;
		
    protected:

        GLuint _shaderProgramID;

        char*  _readShaderFile(const char *filename);                
        GLuint _createShader(const char *shaderFile, GLenum shaderType);        
        GLuint _compileShader(GLuint shader);        
        void   _printCompileError(GLuint shader);        
        GLuint _linkProgram();        
        void   _printLinkError();

		

};

#endif
