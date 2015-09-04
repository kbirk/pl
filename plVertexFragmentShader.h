#ifndef PL_VERTEX_FRAGMENT_SHADER_H
#define PL_VERTEX_FRAGMENT_SHADER_H

#include "plCommon.h"
#include "plShader.h"
#include "plMatrix44.h"
#include "plTexture2D.h"

#define PL_MODEL_MATRIX_UNIFORM_NAME                "uModelMatrix"
#define PL_VIEW_MATRIX_UNIFORM_NAME                 "uViewMatrix"
#define PL_PROJECTION_MATRIX_UNIFORM_NAME           "uProjectionMatrix"
#define PL_COLOUR_UNIFORM_NAME                      "uColour"
#define PL_LIGHT_POSITION_UNIFORM_NAME              "uLightPosition"
#define PL_PICKING_UNIFORM_NAME                     "uPickingColour"
#define PL_TEXTURE_UNIT_0_SAMPLER_UNIFORM_NAME      "uTextureUnit0"
#define PL_TEXTURE_UNIT_1_SAMPLER_UNIFORM_NAME      "uTextureUnit1"
#define PL_TEXTURE_UNIT_2_SAMPLER_UNIFORM_NAME      "uTextureUnit2"
#define PL_TEXTURE_UNIT_3_SAMPLER_UNIFORM_NAME      "uTextureUnit3"


enum plUniformTypeEnum
{
    PL_MODEL_MATRIX_UNIFORM = 0,
    PL_VIEW_MATRIX_UNIFORM,
    PL_PROJECTION_MATRIX_UNIFORM,
    PL_COLOUR_UNIFORM,
    PL_LIGHT_POSITION_UNIFORM,
    PL_PICKING_UNIFORM,
    PL_TEXTURE_UNIT_0_UNIFORM,
    PL_TEXTURE_UNIT_1_UNIFORM,
    PL_TEXTURE_UNIT_2_UNIFORM,
    PL_TEXTURE_UNIT_3_UNIFORM
};


class plVertexFragmentShader : public plShader
{

    public:

        plVertexFragmentShader( const char *vertexFile, const char *fragmentFile );

        void setUniform( const plUniform& uniform ) const;

    private:

        // transformations
        GLint _modelMatrixUniformID;
        GLint _viewMatrixUniformID;
        GLint _projectionMatrixUniformID;
        // colour
        GLint _colourUniformID;
        // light position
        GLint _lightPositionUniformID;
        // picking
        GLint _pickingUniformID;
        // texturing
        GLint _textureUnit0SamplerID;
        GLint _textureUnit1SamplerID;
        GLint _textureUnit2SamplerID;
        GLint _textureUnit3SamplerID;

        void _getUniformLocations();
};

#endif
