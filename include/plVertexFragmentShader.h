#pragma once

#include "plCommon.h"
#include "plShader.h"
#include "plMatrix44.h"
#include "plTexture2D.h"

#define PL_MODEL_MATRIX_UNIFORM_NAME                "uModelMatrix"
#define PL_VIEW_MATRIX_UNIFORM_NAME                 "uViewMatrix"
#define PL_PROJECTION_MATRIX_UNIFORM_NAME           "uProjectionMatrix"
#define PL_COLOR_UNIFORM_NAME                       "uColor"
#define PL_LIGHT_POSITION_UNIFORM_NAME              "uLightPosition"
#define PL_PICKING_UNIFORM_NAME                     "uPickingColor"
#define PL_TEXTURE_UNIT_0_SAMPLER_UNIFORM_NAME      "uTextureUnit0"
#define PL_TEXTURE_UNIT_1_SAMPLER_UNIFORM_NAME      "uTextureUnit1"
#define PL_TEXTURE_UNIT_2_SAMPLER_UNIFORM_NAME      "uTextureUnit2"
#define PL_TEXTURE_UNIT_3_SAMPLER_UNIFORM_NAME      "uTextureUnit3"
#define PL_TEXTURE_UNIT_4_SAMPLER_UNIFORM_NAME      "uTextureUnit4"

enum plUniformTypeEnum
{
    PL_MODEL_MATRIX_UNIFORM = 0,
    PL_VIEW_MATRIX_UNIFORM,
    PL_PROJECTION_MATRIX_UNIFORM,
    PL_COLOR_UNIFORM,
    PL_LIGHT_POSITION_UNIFORM,
    PL_PICKING_UNIFORM,
};

enum plTextureUnitEnum
{
    PL_TEXTURE_UNIT_0,
    PL_TEXTURE_UNIT_1,
    PL_TEXTURE_UNIT_2,
    PL_TEXTURE_UNIT_3,
    PL_TEXTURE_UNIT_4
};

class plVertexFragmentShader : public plShader
{
    public:

        plVertexFragmentShader(const char* vertexFile, const char* fragmentFile);

        void setTexture(uint32_t type, const std::shared_ptr<plTexture2D>& texture) const;
        void setUniform(const plUniform& uniform) const;

    private:

        // transformations
        GLint _modelMatrixUniformID;
        GLint _viewMatrixUniformID;
        GLint _projectionMatrixUniformID;
        // color
        GLint _colorUniformID;
        // light position
        GLint _lightPositionUniformID;
        // picking
        GLint _pickingUniformID;
        // texturing
        GLint _textureUnit0SamplerID;
        GLint _textureUnit1SamplerID;
        GLint _textureUnit2SamplerID;
        GLint _textureUnit3SamplerID;
        GLint _textureUnit4SamplerID;

        void _getUniformLocations();
};
