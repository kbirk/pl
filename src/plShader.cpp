#include "plShader.h"

plShader::plShader()
    : _id(0)
    , _good(true)
{
}

plShader::~plShader()
{
    glDeleteProgram(_id);
    LOG_OPENGL("glDeleteProgram");
}

GLuint plShader::_createShader(const std::string& shaderFile, GLenum shaderType)
{
    // load shader file into memory
    const char* shaderSource = _readShaderFile(shaderFile); // allocate memory to shaderSource
    // check for error
    if (!shaderSource) {
        delete[] shaderSource;
        return 0;
    }
    // create shader object
    GLuint shader = glCreateShader(shaderType);
    LOG_OPENGL("glCreateShader");
    // set source code of shader object
    glShaderSource(shader, 1, &shaderSource, nullptr);
    LOG_OPENGL("glShaderSource");
    delete[] shaderSource; // deallocate memory from shaderSource
    // return shader object
    return shader;
}

GLuint plShader::_createShader(const std::vector<std::string>& shaderFiles, GLenum shaderType)
{
    std::vector<const char*> sources;

    for (uint32_t i = 0; i < shaderFiles.size(); i++) {
        // load shader file into memory
        const char* shaderSource = _readShaderFile(shaderFiles[i]); // allocate memory to shaderSource

        // check for error
        if (!shaderSource) {
            // deallocate previous arrays
            for (uint32_t j = 0; j < i; j++) {
                delete[] sources[j];
            }
            return 0;
        }
        sources.push_back(_readShaderFile(shaderFiles[i])); // allocate memory to shaderSource
    }

    // create shader object
    GLuint shader = glCreateShader(shaderType);
    LOG_OPENGL("glCreateShader");

    // set source code of shader object
    glShaderSource(shader, sources.size(), &sources[0], nullptr);
    LOG_OPENGL("glShaderSource");
    for (uint32_t i = 0; i < shaderFiles.size(); i++) {
        delete[] sources[i]; // deallocate memory from shaderSource
    }

    // return shader object
    return shader;
}

char* plShader::_readShaderFile(const std::string& filename)
{
    // THIS FUNCTION DOES NOT DE-ALLOCATE HEAP MEMORY!
    std::ifstream stream(filename, std::ifstream::in | // input stream
            std::ifstream::binary | // consider stream as binary
            std::ifstream::ate); // set position indicator at end of stream
    char* string;
    if (stream.is_open()) {
        std::streamoff size = stream.tellg();
        string = new char[static_cast<uint32_t>(size) + 1]; // ** allocates memory but does NOT deallocate **
        stream.seekg(0, std::ios::beg); // set position indicator back to beginning of stream
        stream.read(string, size);
        stream.close();
        string[size] = '\0'; // nullptr terminate
        return string;
    }

    // print error message
    LOG_WARN("Could not open shader file: " << filename);
    return nullptr;
}

bool plShader::_compileShader(GLuint shader)
{
    // compile vertex shader
    glCompileShader(shader);
    LOG_OPENGL("glCompileShader");
    // error check
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    LOG_OPENGL("glGetShaderiv");

    if (result == GL_FALSE) {
        // print error
        _printCompileError(shader);
        // delete current objects and abort constructor
        glDeleteShader(shader);
        LOG_OPENGL("glDeleteShader");
        _id = 0;
        _good = false;
        return false;
    }
    return true;
}

GLuint plShader::_createAndCompileShader(const std::string& shaderFile, GLenum shaderType)
{
    // create shader object
    GLuint shader = _createShader(shaderFile, shaderType);
    // compile shader object, check for error
    if (!_compileShader(shader))
        return 0;
    return shader;
}

GLuint plShader::_createAndCompileShader(const std::vector<std::string>& shaderFiles, GLenum shaderType)
{
    // create shader object
    GLuint shader = _createShader(shaderFiles, shaderType);
    // compile shader object, check for error
    if (!_compileShader(shader))
        return 0;
    return shader;
}

void plShader::_printCompileError(GLuint shader)
{
    // get info log length
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    LOG_OPENGL("glGetShaderiv");
    // get error message
    std::vector<char> msg(infoLogLength);
    glGetShaderInfoLog(shader, infoLogLength, nullptr, &msg[0]);
    LOG_OPENGL("glGetShaderInfoLog");
    // print error message
    LOG_ERROR("Compilation errors: " << std::string(msg.begin(), msg.end()));
}

bool plShader::_linkProgram()
{
    // link shader program
    glLinkProgram(_id);
    LOG_OPENGL("glLinkProgram");
    // error check
    GLint result;
    glGetProgramiv(_id, GL_LINK_STATUS, &result);
    LOG_OPENGL("glGetProgramiv");
    // check for error
    if (result == GL_FALSE) {
        // print error
        _printLinkError();
        _id = 0;
        _good = false;
        return false;
    }
    return true;
}

void plShader::_printLinkError()
{
    // get info log length
    GLint infoLogLength;
    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
    LOG_OPENGL("glGetProgramiv");
    // get error message
    std::vector<char> msg(infoLogLength);
    glGetProgramInfoLog(_id, infoLogLength, nullptr, &msg[0]);
    LOG_OPENGL("glGetProgramInfoLog");
    // print error message
    LOG_ERROR("Linking errors: " << std::string(msg.begin(), msg.end()));
}
