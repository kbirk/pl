#include "plVertexFragmentShader.h"

plVertexFragmentShader::plVertexFragmentShader(const char *vertexFile, const char *fragmentFile)
{
    // create and compile shader, tests for errors
    GLuint vertexShader = _createAndCompileShader(vertexFile,   GL_VERTEX_SHADER);
    GLuint fragmentShader = _createAndCompileShader(fragmentFile, GL_FRAGMENT_SHADER);

    // create program
    _id = glCreateProgram();

    // attach shaders
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);

    // link program, check for error
    _linkProgram();

    // free shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // get uniform locations
    _getUniformLocations();
}


void plVertexFragmentShader::_getUniformLocations()
{
    // transformations
    _modelMatrixUniformID      = glGetUniformLocation(_id, PL_MODEL_MATRIX_UNIFORM_NAME);
    _viewMatrixUniformID       = glGetUniformLocation(_id, PL_VIEW_MATRIX_UNIFORM_NAME);
    _projectionMatrixUniformID = glGetUniformLocation(_id, PL_PROJECTION_MATRIX_UNIFORM_NAME);
    // color
    _colorUniformID            = glGetUniformLocation(_id, PL_COLOR_UNIFORM_NAME);
    // light position
    _lightPositionUniformID    = glGetUniformLocation(_id, PL_LIGHT_POSITION_UNIFORM_NAME);
    // picking
    _pickingUniformID          = glGetUniformLocation(_id, PL_PICKING_UNIFORM_NAME);
    // texturing
    _textureUnit0SamplerID     = glGetUniformLocation(_id, PL_TEXTURE_UNIT_0_SAMPLER_UNIFORM_NAME);
    _textureUnit1SamplerID     = glGetUniformLocation(_id, PL_TEXTURE_UNIT_1_SAMPLER_UNIFORM_NAME);
    _textureUnit2SamplerID     = glGetUniformLocation(_id, PL_TEXTURE_UNIT_2_SAMPLER_UNIFORM_NAME);
    _textureUnit3SamplerID     = glGetUniformLocation(_id, PL_TEXTURE_UNIT_3_SAMPLER_UNIFORM_NAME);
    _textureUnit4SamplerID     = glGetUniformLocation(_id, PL_TEXTURE_UNIT_4_SAMPLER_UNIFORM_NAME);
}


void plVertexFragmentShader::setTexture(uint32_t type, const std::shared_ptr<plTexture2D>& texture) const
{
    switch (type)
    {
        case PL_TEXTURE_UNIT_0:
            if (_textureUnit0SamplerID != -1)
            {
                // activate unit
                glActiveTexture(GL_TEXTURE0);
                // bind the texture
                texture->bind();
                // set sampler index
                glUniform1i(_textureUnit0SamplerID, 0);
            }
            break;

        case PL_TEXTURE_UNIT_1:
            if (_textureUnit1SamplerID != -1)
            {
                // activate unit
                glActiveTexture(GL_TEXTURE1);
                // bind the texture
                texture->bind();
                // set sampler index
                glUniform1i(_textureUnit1SamplerID, 1);
            }
            break;

        case PL_TEXTURE_UNIT_2:
            if (_textureUnit2SamplerID != -1)
            {
                // activate unit
                glActiveTexture(GL_TEXTURE2);
                // bind the texture
                texture->bind();
                // set sampler index
                glUniform1i(_textureUnit2SamplerID, 2);
            }
            break;

        case PL_TEXTURE_UNIT_3:
            if (_textureUnit3SamplerID != -1)
            {
                // activate unit
                glActiveTexture(GL_TEXTURE3);
                // bind the texture
                texture->bind();
                // set sampler index
                glUniform1i(_textureUnit3SamplerID, 3);
            }
            break;

        case PL_TEXTURE_UNIT_4:
            if (_textureUnit4SamplerID != -1)
            {
                // activate unit
                glActiveTexture(GL_TEXTURE4);
                // bind the texture
                texture->bind();
                // set sampler index
                glUniform1i(_textureUnit4SamplerID, 4);
            }
            break;
    }
}


void plVertexFragmentShader::setUniform(uint32_t type, const std::shared_ptr<plUniform>& uniform) const
{
    switch (type)
    {
        // transformations
        case PL_MODEL_MATRIX_UNIFORM:
        {
            if (_modelMatrixUniformID != -1)
            {
                glUniformMatrix4fv(
                    _modelMatrixUniformID,
                    1,
                    GL_FALSE,
                    (GLfloat*)(uniform->data()));
            }
            break;
        }
        case PL_VIEW_MATRIX_UNIFORM:
        {
            if (_viewMatrixUniformID != -1)
            {
                glUniformMatrix4fv(
                    _viewMatrixUniformID,
                    1,
                    GL_FALSE,
                    (GLfloat*)(uniform->data()));
            }
            break;
        }
        case PL_PROJECTION_MATRIX_UNIFORM:
        {
            if (_projectionMatrixUniformID != -1)
            {
                glUniformMatrix4fv(
                    _projectionMatrixUniformID,
                    1,
                    GL_FALSE,
                    (GLfloat*)(uniform->data()));
            }
            break;
        }

        // color
        case PL_COLOR_UNIFORM:
        {
            if (_colorUniformID != -1)
            {
                glUniform4fv(
                    _colorUniformID,
                    1,
                    (GLfloat*)(uniform->data()));
            }
            break;
        }

        // lighting
        case PL_LIGHT_POSITION_UNIFORM:
        {
            if (_lightPositionUniformID != -1)
            {
                glUniform3fv(
                    _lightPositionUniformID,
                    1,
                    (GLfloat*)(uniform->data()));
            }
            break;
        }

        // picking
        case PL_PICKING_UNIFORM:
        {
            if (_pickingUniformID != -1)
            {
                glUniform3iv(
                    _pickingUniformID,
                    1,
                    (GLint*)(uniform->data()));
            }
            break;
        }
    }
}
