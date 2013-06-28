#ifndef __PL_SHADER_H__
#define __PL_SHADER_H__

#include <vector>
#include "pl.h"

// TODO: switch to layout qualifiers!!!!
/*
enum ARRAY_ATTRIBUTE
{ 
	POSITION_ATTRIBUTE = 0,
	NORMAL_ATTRIBUTE,
    TANGENT_ATTRIBUTE,
    BITANGENT_ATTRIBUTE,
	TEXTURE_ATTRIBUTE0,
	TEXTURE_ATTRIBUTE1,
	TEXTURE_ATTRIBUTE2,
	TEXTURE_ATTRIBUTE3
};
*/

class plShader 
{
    public:                    
          
        plShader(const char *shaderFile, GLenum shaderType)
        {
            // create compute shader object 
            GLuint shader = createShader(shaderFile, shaderType);
            // compile compute shader object, check for error
            if(compileShader(shader) == GL_FALSE)
			{			
                // print error
                printCompileError(shader);
                // delete current objects and abort constructor
				glDeleteShader(shader);
				_shaderProgramID = (GLuint)NULL;               
				return;
			}

			// create program
			_shaderProgramID = glCreateProgram();
			// attach shader 		
			glAttachShader(_shaderProgramID, shader);
			
            // link program, check for error
			if(linkProgram() == GL_FALSE)
			{
                // print error
                printLinkError();                
				_shaderProgramID = (GLuint)NULL;
			}

			// free shader objects
			glDeleteShader(shader);  
        
        }  
          
        plShader(const char *vertexFile, const char *fragmentFile)
		{         
            // create vertex shader object 
            GLuint vertexShader = createShader(vertexFile, GL_VERTEX_SHADER);
            // compile vertex shader object, check for error
            if(compileShader(vertexShader) == GL_FALSE)
			{			
                // print error
                printCompileError(vertexShader);
                // delete current objects and abort constructor
				glDeleteShader(vertexShader);
				_shaderProgramID = (GLuint)NULL;               
				return;
			}
            // create fragment shader object
            GLuint fragmentShader = createShader(fragmentFile, GL_FRAGMENT_SHADER);
            // compile fragment shader object, check for error
            if(compileShader(fragmentShader) == GL_FALSE)
			{
				// print error
                printCompileError(fragmentShader);
                // delete current objects and abort constructor
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
				_shaderProgramID = (GLuint)NULL;
				return;
			} 
			// create program
			_shaderProgramID = glCreateProgram();
			// attach shaders
			glAttachShader(_shaderProgramID, vertexShader);
			glAttachShader(_shaderProgramID, fragmentShader);
			
			// bind attribute indices, bind fragment shader outputs
            // ** must be done before linking ** 
            // no issues if specific attribute is not present in shader
            //bindAttributeLocations();   
            //bindFragDataLocations();    

            // link program, check for error
			if(linkProgram() == GL_FALSE)
			{
                // print error
                printLinkError();                
				_shaderProgramID = (GLuint)NULL;
			}

			// free shader objects
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);   
		}

		~plShader()
		{
			glDeleteProgram(_shaderProgramID);
		}

        char* readShaderFile(const char *filename)
        {
            // THIS FUNCTION DOES NOT DE-ALLOCATE HEAP MEMORY!
            std::ifstream stream (filename, std::ifstream::in |         // input stream
                                            std::ifstream::binary |     // consider stream as binary 
                                            std::ifstream::ate);        // set position indicator at end of stream
            char *string;
            if (stream.is_open())
            {				
                std::streamoff size = stream.tellg();
                string = new char[static_cast<PLuint>(size)+1];	// ** allocates memory but does NOT deallocate **
                stream.seekg(0, std::ios::beg);                         // set position indicator back to beginning of stream
                stream.read(string, size);
                stream.close();
                string[size] = '\0';	                                // null terminate
                return string;
            }
            // print error message
			fprintf(stdout, "Error opening shader file");
            return NULL;
        }
        
        GLuint createShader(const char *shaderFile, GLenum shaderType)
        {
            // load shader file into memory
			const char *shaderSource = readShaderFile(shaderFile);      // allocate memory to shaderSource 
            // create shader object
			GLuint shader = glCreateShader(shaderType);
            // set source code of shader object
			glShaderSource(shader, 1, &shaderSource, NULL);
            delete [] shaderSource;                                     // deallocate memory from shaderSource
            // return shader object            
            return shader;          
        }
        
        GLuint compileShader(GLuint shader)
        {
            // compile vertex shader
			glCompileShader(shader);          
			// error check
            GLint result;           
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);          
            // return error code
            return result; 
        }
        
        void printCompileError(GLuint shader)
        {
            // get info log length
            GLint infoLogLength;    
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            // get error message
            std::vector<char> errorMessage(infoLogLength);  
            glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMessage[0]);
            // print error message
			fprintf(stdout, "%s\n", &errorMessage[0]);
        }
        
        GLuint linkProgram()
        {
            // link shader program
			glLinkProgram(_shaderProgramID);           
			// error check
            GLint result; 
			glGetProgramiv(_shaderProgramID, GL_LINK_STATUS, &result);
            // return error code
            return result; 
        }
        
        void printLinkError()
        {
            // get info log length
            GLint infoLogLength;
            glGetShaderiv(_shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
            // get error message
            std::vector<char> errorMessage(infoLogLength);
			glGetShaderInfoLog(_shaderProgramID, infoLogLength, NULL, &errorMessage[0]);
            // print error message
			fprintf(stdout, "%s\n", &errorMessage[0]);
        }
        /*
        void bindAttributeLocations() 
        {
            // bind all available attribute locations, doesnt seem to complain if they don't exist
            glBindAttribLocation(_shaderProgramID, POSITION_ATTRIBUTE,  "vPosition" );
			glBindAttribLocation(_shaderProgramID, NORMAL_ATTRIBUTE, 	 "vNormal"   );
            glBindAttribLocation(_shaderProgramID, TANGENT_ATTRIBUTE, 	 "vTangent"  );
            glBindAttribLocation(_shaderProgramID, BITANGENT_ATTRIBUTE, "vBiTangent");
			glBindAttribLocation(_shaderProgramID, TEXTURE_ATTRIBUTE0,  "vTexCoord0");
            glBindAttribLocation(_shaderProgramID, TEXTURE_ATTRIBUTE1,  "vTexCoord1");
            glBindAttribLocation(_shaderProgramID, TEXTURE_ATTRIBUTE2,  "vTexCoord2");
            glBindAttribLocation(_shaderProgramID, TEXTURE_ATTRIBUTE3,  "vTexCoord3");
        }
        
        
        void bindFragDataLocations()
        {      
			// any name may be specified in name, including a name that is never
			// used as a varying out variable in any fragment shader object
			// ONLY ORDER IS IMPORTANT, MUST BE SAME IN SPECIFIC SHADER!
            glBindFragDataLocation(_shaderProgramID, 0, "positionOutput");
            glBindFragDataLocation(_shaderProgramID, 1, "normalOutput"  );        
            glBindFragDataLocation(_shaderProgramID, 2, "diffuseOutput" );
            glBindFragDataLocation(_shaderProgramID, 3, "specularOutput");
            glBindFragDataLocation(_shaderProgramID, 4, "blendOutput"   );
			glBindFragDataLocation(_shaderProgramID, 5, "shadowOutput"  );
            glBindFragDataLocation(_shaderProgramID, 6, "colourOutput"  );
			
        }
        */
        
		void bind() const
		{
			// always call this before sending uniforms!            
            glUseProgram(_shaderProgramID);  
		}
		
		void unbind() const
		{          
            glUseProgram(0);  
		}
		
    protected:

		GLuint _shaderProgramID;

};

#endif
