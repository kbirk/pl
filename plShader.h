#ifndef PL_SHADER_H
#define PL_SHADER_H

#include "plCommon.h"

#include "plString.h"

// attribute locations, set by VBOs and mirrored in shader files
#define PL_POSITION_ATTRIBUTE  0
#define PL_NORMAL_ATTRIBUTE    1
#define PL_COLOUR_ATTRIBUTE    2
#define PL_TEXCOORD_ATTRIBUTE  3

class plShader 
{

    public:                    
          
        plShader( const plString &shaderFile, GLenum shaderType);      
        plShader( const std::vector< plString > &shaderFiles, GLenum shaderType );      
        plShader( const plString &vertexFile, const plString &fragmentFile);

		~plShader();
                
		void bind()   const { glUseProgram(_shaderProgramID);  }	    
		void unbind() const { glUseProgram(0);                 }
		
		bool good() const { return _good; }
		
		virtual void getUniformLocations() = 0;
		
    protected:

        GLuint _shaderProgramID;
        bool   _good;
        
        char*  _readShaderFile( const plString &filename );                
        GLuint _createShader( const plString &shaderFile, GLenum shaderType ); 
        GLuint _createShader( const std::vector<plString> &shaderFiles, GLenum shaderType );
        GLuint _compileShader(GLuint shader);        
        void   _printCompileError(GLuint shader);        
        GLuint _linkProgram();        
        void   _printLinkError();

};

std::string mapGLErrorToString( GLenum errorCode );
void reportOpenGLError        ( const std::string &str );
void checkOpenGLImplementation();   


#endif
