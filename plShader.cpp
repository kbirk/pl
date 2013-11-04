#include "plShader.h"

plShader::plShader( const plString &shaderFile, GLenum shaderType )
{
    // create compute shader object 
    GLuint shader = _createShader(shaderFile, shaderType);
    // compile compute shader object, check for error
    if( _compileShader(shader) == GL_FALSE )
	{			
        // print error
        _printCompileError(shader);
        // delete current objects and abort constructor
		glDeleteShader(shader);
		_shaderProgramID = (GLuint)NULL;     
		_good = false;          
		return;
	}

	// create program
	_shaderProgramID = glCreateProgram();
	// attach shader 		
	glAttachShader(_shaderProgramID, shader);
	
    // link program, check for error
	if( _linkProgram() == GL_FALSE )
	{
        // print error
        _printLinkError();                
		_shaderProgramID = (GLuint)NULL;
		_good = false;   
	}

	// free shader objects
	glDeleteShader(shader);  
		
	_good = true;        
}  
  
  
plShader::plShader( const std::vector< plString > &shaderFiles, GLenum shaderType )
{
    // create compute shader object 
    GLuint shader = _createShader(shaderFiles, shaderType);
    // compile compute shader object, check for error
    if( _compileShader(shader) == GL_FALSE )
	{			
        // print error
        _printCompileError(shader);
        // delete current objects and abort constructor
		glDeleteShader(shader);
		_shaderProgramID = (GLuint)NULL;     
		_good = false;            
		return;
	}

	// create program
	_shaderProgramID = glCreateProgram();
	// attach shader 		
	glAttachShader(_shaderProgramID, shader);
	
    // link program, check for error
	if( _linkProgram() == GL_FALSE )
	{
        // print error
        _printLinkError();                
		_shaderProgramID = (GLuint)NULL;
		_good = false;   
	}

	// free shader objects
	glDeleteShader(shader);    
			
	_good = true;
}  
  
  
plShader::plShader( const plString &vertexFile, const plString &fragmentFile)
{         
    // create vertex shader object 
    GLuint vertexShader = _createShader(vertexFile, GL_VERTEX_SHADER);
    // compile vertex shader object, check for error
    if(_compileShader(vertexShader) == GL_FALSE)
	{			
        // print error
        _printCompileError(vertexShader);
        // delete current objects and abort constructor
		glDeleteShader(vertexShader);
		_shaderProgramID = (GLuint)NULL;
		_good = false;                  
		return;
	}
    // create fragment shader object
    GLuint fragmentShader = _createShader(fragmentFile, GL_FRAGMENT_SHADER);
    // compile fragment shader object, check for error
    if(_compileShader(fragmentShader) == GL_FALSE)
	{
		// print error
        _printCompileError(fragmentShader);
        // delete current objects and abort constructor
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		_shaderProgramID = (GLuint)NULL;
		_good = false;   
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
	if(_linkProgram() == GL_FALSE)
	{
        // print error
        _printLinkError();                
		_shaderProgramID = (GLuint)NULL;
		_good = false;   
	}

	// free shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);   
	
	_good = true;
}


plShader::~plShader()
{
	glDeleteProgram(_shaderProgramID);
}


GLuint plShader::_createShader( const plString &shaderFile, GLenum shaderType)
{
    // load shader file into memory
	const char *shaderSource = _readShaderFile(shaderFile);     // allocate memory to shaderSource 

    // check for error
    if (!shaderSource)
        return 0;

    // create shader object
	GLuint shader = glCreateShader(shaderType);
    // set source code of shader object
	glShaderSource( shader, 1, &shaderSource, NULL );
    delete [] shaderSource;                                     // deallocate memory from shaderSource
    // return shader object            
    return shader;          
}


GLuint plShader::_createShader( const std::vector<plString> &shaderFiles, GLenum shaderType )
{
    std::vector<const char*> sources;

    for (PLuint i=0; i< shaderFiles.size(); i++)
    {
        // load shader file into memory
        const char *shaderSource = _readShaderFile( shaderFiles[i] );  // allocate memory to shaderSource 
        // check for error
        if (!shaderSource)
        {
            for (PLuint j=0; j<i; j++)
            {
                delete [] sources[i];
            }
            return 0;
        }
        sources.push_back( _readShaderFile( shaderFiles[i] ) );  // allocate memory to shaderSource 
    }
    
    // create shader object
	GLuint shader = glCreateShader(shaderType);

    // set source code of shader object
	glShaderSource( shader, sources.size(), &sources[0], NULL );
    for (PLuint i=0; i< shaderFiles.size(); i++)
    {
        delete [] sources[i];  // deallocate memory from shaderSource
    }

    // return shader object            
    return shader;          
}


char* plShader::_readShaderFile( const plString &filename )
{
    // THIS FUNCTION DOES NOT DE-ALLOCATE HEAP MEMORY!
    std::ifstream stream (filename, std::ifstream::in |         // input stream
                                    std::ifstream::binary |     // consider stream as binary 
                                    std::ifstream::ate);        // set position indicator at end of stream
    char *string;
    if (stream.is_open())
    {				
        std::streamoff size = stream.tellg();
        string = new char[static_cast<PLuint>(size)+1];	        // ** allocates memory but does NOT deallocate **
        stream.seekg(0, std::ios::beg);                         // set position indicator back to beginning of stream
        stream.read(string, size);
        stream.close();
        string[size] = '\0';	                                // null terminate
        return string;
    }

    // print error message
    std::cerr << "plShader::_readShaderFile() error: could not open shaderfile: " << filename << std::endl;
    return NULL;
}


GLuint plShader::_compileShader(GLuint shader)
{
    // compile vertex shader
	glCompileShader(shader);          
	// error check
    GLint result;           
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);          
    // return error code
    return result; 
}


void plShader::_printCompileError(GLuint shader)
{
    // get info log length
    GLint infoLogLength;    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    // get error message
    std::vector<char> errorMessage(infoLogLength);  
    glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMessage[0]);
    // print error message
	fprintf(stdout, "Compilation errors:\n%s\n", &errorMessage[0]);
}


GLuint plShader::_linkProgram()
{
    // link shader program
	glLinkProgram(_shaderProgramID);           
	// error check
    GLint result; 
	glGetProgramiv(_shaderProgramID, GL_LINK_STATUS, &result );
    // return error code
    return result; 
}


void plShader::_printLinkError()
{
    // get info log length
    GLint infoLogLength;
    glGetProgramiv( _shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength );
    // get error message
    std::vector<char> errorMessage(infoLogLength);
	glGetProgramInfoLog( _shaderProgramID, infoLogLength, NULL, &errorMessage[0] );
    // print error message
	fprintf(stdout, "Linking errors:\n%s\n", &errorMessage[0]);
}



std::string mapGLErrorToString( GLenum errorCode )
{
    switch ( errorCode )
    {
        case GL_NO_ERROR:                       return "No error has been recorded.";            
        case GL_INVALID_ENUM:                   return "GL_INVALID_ENUM Error: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";                  
        case GL_INVALID_VALUE:                  return "GL_INVALID_VALUE Error: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
        case GL_INVALID_OPERATION:              return "GL_INVALID_OPERATION Error: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
        case GL_INVALID_FRAMEBUFFER_OPERATION:  return "GL_INVALID_FRAMEBUFFER_OPERATION Error: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
        case GL_OUT_OF_MEMORY:                  return "GL_OUT_OF_MEMORY Error: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
        case GL_STACK_UNDERFLOW:                return "GL_STACK_UNDERFLOW Error: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
        case GL_STACK_OVERFLOW:                 return "GL_STACK_OVERFLOW Error: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    }
    return "";
}


void reportOpenGLError( const std::string &str ) 
{
    GLuint errnum;
    while ( errnum = glGetError() ) 
    {
        std::cout << str << " " << mapGLErrorToString( errnum ) << std::endl;
    }
}


void checkOpenGLImplementation()
{
    GLint data[256];
    std::cout << "\n-------- OpenGL Compute Shader Uniform and Shader Storage blocks: ------- \n" <<
                   "-------------------------------------------------------------------------- \n"  << std::endl;

        glGetIntegerv( GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, data );    
        std::cout << "\tGL_MAX_SHADER_STORAGE_BUFFER_BINDINGS: " << data[0] << std::endl;

        glGetIntegerv( GL_MAX_SHADER_STORAGE_BLOCK_SIZE, data );    
        std::cout << "\tGL_MAX_SHADER_STORAGE_BLOCK_SIZE: " << data[0] << std::endl;

        glGetIntegerv( GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, data );    
        std::cout << "\tGL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS: " << data[0] << std::endl;

        glGetIntegerv( GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, data );    
        std::cout << "\tGL_MAX_COMBINED_SHADER_STORAGE_BLOCKS: " << data[0] << std::endl;

        glGetIntegerv( GL_MAX_COMPUTE_UNIFORM_BLOCKS, data );    
        std::cout << "\tGL_MAX_COMPUTE_UNIFORM_BLOCKS: " << data[0] << std::endl;

        glGetIntegerv( GL_MAX_COMPUTE_UNIFORM_COMPONENTS, data );    
        std::cout << "\tGL_MAX_COMPUTE_UNIFORM_COMPONENTS: " << data[0] << std::endl;
    
        glGetIntegerv( GL_MAX_COMBINED_UNIFORM_BLOCKS, data );    
        std::cout << "\tGL_MAX_COMBINED_UNIFORM_BLOCKS: " << data[0] << std::endl;
        
        glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, data );    
        std::cout << "\tGL_MAX_UNIFORM_BLOCK_SIZE: " << data[0] << std::endl;
        
        glGetIntegerv( GL_MAX_UNIFORM_LOCATIONS, data );    
        std::cout << "\tGL_MAX_UNIFORM_LOCATIONS: " << data[0] << std::endl;
    
        std::cout << "\n----------- OpenGL Compute Shader Invocations and work Groups: ----------- \n" <<
                       "-------------------------------------------------------------------------- \n"  << std::endl;

        glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, data );    
        std::cout << "\tGL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: " << data[0] << std::endl;

        glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &data[0] );    
        glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &data[1] );  
        glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &data[2] );  
        std::cout << "\tGL_MAX_COMPUTE_WORK_GROUP_COUNT (x): " << data[0] << std::endl;
        std::cout << "\t                                (y): " << data[1] << std::endl;
        std::cout << "\t                                (z): " << data[2] << std::endl;

        glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &data[0] );    
        glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &data[1] );  
        glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &data[2] );  
        std::cout << "\tGL_MAX_COMPUTE_WORK_GROUP_SIZE (x): " << data[0] << std::endl;
        std::cout << "\t                               (y): " << data[1] << std::endl;
        std::cout << "\t                               (z): " << data[2] << std::endl;

        std::cout << "\n-------------------------------------------------------------------------- \n" <<
                       "-------------------------------------------------------------------------- \n"  << std::endl;
}
