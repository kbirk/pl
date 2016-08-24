#pragma once

#include "plCommon.h"
#include "plString.h"
#include "plOpenGLInfo.h"
#include "plTexture2D.h"
#include "plUniform.h"

class plShader
{
    public:

        plShader();
        ~plShader();

        void bind() const
        {
            glUseProgram(_id);
            LOG_OPENGL("glUseProgram");
        }
        void unbind() const
        {
            glUseProgram(0);
            LOG_OPENGL("glUseProgram");  
        }

        bool good() const   { return _good; }
        uint32_t id() const { return _id;   }

        virtual void setUniform(uint32_t type, const std::shared_ptr<plUniform>& uniform) const {};
        virtual void setTexture(uint32_t type, const std::shared_ptr<plTexture2D>& texture) const {};

    protected:

        GLuint _id;
        bool _good;

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
