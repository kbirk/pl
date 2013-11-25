#ifndef PL_OPEN_GL_INFO_H
#define PL_OPEN_GL_INFO_H

#include "plCommon.h"

namespace plOpenGLInfo 
{

    void init();
    void print(); 
    PLuint sizeOfType( PLuint enumType );
    void reportError( const std::string &str );

    extern GLint maxComputeShaderStorageBlocks,
                  maxCombinedShaderStorageBlocks,
                  maxComputeUniformBlocks,
                  maxComputeTextureImageUnits,
                  maxComputeUniformComponents,
                  maxComputeAtomicCounters,
                  maxComputeAtomicCounterBuffers,
                  maxCombinedComputeUniformComponents,
                  maxComputeWorkGroupInvocations; 
                   
    extern GLint maxComputeWorkGroupCount[3];
    extern GLint maxComputeWorkGroupSize[3];
    
    extern GLint maxDebugGroupStackDepth,
                  majorVersion,
                  max3DTextureSize,
                  maxArrayTextureLayers,
                  maxClipDistances,
                  maxColorTextureSamples,
                  maxColorAttachments,
                  maxCombinedAtomicCounters,
                  maxCombinedFragmentUniformComponents,
                  maxCombinedGeometryUniformComponents,
                  maxCombinedTextureImageUnits,
                  maxCombinedUniformBlocks,
                  maxCombinedVertexUniformComponents,
                  maxCubeMapTextureSize,
                  maxDepthTextureSamples,
                  maxDrawBuffers,
                  maxDualSourceDrawBuffers,
                  maxElementsIndices,
                  maxElementsVertices,
                  maxFragmentAtomicCounters,
                  maxFragmentShaderStorageBlocks,
                  maxFragmentInputComponents,
                  maxFragmentUniformComponents,
                  maxFragmentUniformVectors,
                  maxFragmentUniformBlocks,
                  maxFramebufferWidth,
                  maxFramebufferHeight,
                  maxFramebufferLayers,
                  maxFramebufferSamples,
                  maxGeometryAtomicCounters,
                  maxGeometryShaderStorageBlocks,
                  maxGeometryInputComponents,
                  maxGeometryOutputComponents,
                  maxGeometryTextureImageUnits,
                  maxGeometryUniformBlocks,
                  maxGeometryUniformComponents,
                  maxIntegerSamples,
                  minMapBufferAlignment,
                  maxLabelLength,
                  maxProgramTexelOffset,
                  minProgramTexelOffset,
                  maxRectangleTextureSize,
                  maxRenderbufferSize,
                  maxSampleMaskWords;
          
    extern GLint64 maxServerWaitTimeout;
    
    extern GLint maxShaderStorageBufferBindings,
                  maxTessControlAtomicCounters,
                  maxTessEvaluationAtomicCounters,
                  maxTessControlShaderStorageBlocks,
                  maxTessEvaluationShaderStorageBlocks,
                  maxTextureBufferSize,
                  maxTextureImageUnits,
                  maxTextureLODBias,
                  maxTextureSize,
                  maxUniformBufferBindings,
                  maxUniformBlockSize,
                  maxUniformLocations,
                  maxVaryingComponents,
                  maxVaryingVectors,
                  maxVaryingFloats,
                  maxVertexAtomicCounters,
                  maxVertexAttribs,
                  maxVertexShaderStorageBlocks,
                  maxVertexTextureImageUnits,
                  maxVertexUniformComponents,
                  maxVertexUniformVectors,
                  maxVertexOutputComponents,
                  maxVertexUniformBlocks;
          
    extern GLint maxViewportDims[2];
    extern GLint maxViewports,
              minorVersion, 
              numCompressedTextureFormats,
              numExtensions, 
              numProgramBinaryFormats, 
              numShaderBinaryFormats,
              maxVertexAttribRelativeOffset,
              maxVertexAttribBindings,
              maxElementIndex;

}

#endif
