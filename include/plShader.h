#pragma once

#include "plCommon.h"
#include "plString.h"
#include "plOpenGLInfo.h"
#include "plUniform.h"

class plShader
{
    public:

        plShader();
        ~plShader();

        void bind() const   { glUseProgram(_id);  }
        void unbind() const { glUseProgram(0);    }

        bool good() const   { return _good; }
        uint32_t id() const { return _id;   }

        virtual void setUniform(const plUniform& uniform) const {};

    protected:

        GLuint _id;
        bool   _good;

        char* _readShaderFile(const std::string &filename);
        GLuint _createShader(const std::string &shaderFile, GLenum shaderType);
        GLuint _createShader(const std::vector<std::string> &shaderFiles, GLenum shaderType);
        bool _compileShader(GLuint shader);
        GLuint _createAndCompileShader(const std::string &shaderFile, GLenum shaderType);
        GLuint _createAndCompileShader(const std::vector<std::string> &shaderFiles, GLenum shaderType);
        void _printCompileError(GLuint shader);
        bool _linkProgram();
        void _printLinkError();

        virtual void _getUniformLocations() = 0;
};
