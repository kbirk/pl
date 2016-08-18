#include "plOpenGLInfo.h"

namespace plOpenGLInfo
{

    GLint maxComputeShaderStorageBlocks,
          maxCombinedShaderStorageBlocks,
          maxComputeUniformBlocks,
          maxComputeTextureImageUnits,
          maxComputeUniformComponents,
          maxComputeAtomicCounters,
          maxComputeAtomicCounterBuffers,
          maxCombinedComputeUniformComponents,
          maxComputeWorkGroupInvocations;

    GLint maxComputeWorkGroupCount[3];
    GLint maxComputeWorkGroupSize[3];

    GLint maxDebugGroupStackDepth,
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

    GLint64 maxServerWaitTimeout;

    GLint maxShaderStorageBufferBindings,
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

    GLint maxViewportDims[2];
    GLint maxViewports,
          minorVersion,
          numCompressedTextureFormats,
          numExtensions,
          numProgramBinaryFormats,
          numShaderBinaryFormats,
          maxVertexAttribRelativeOffset,
          maxVertexAttribBindings,
          maxElementIndex;

    void init()
    {
        glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS,
                &maxComputeShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS,
                &maxCombinedShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &maxComputeUniformBlocks); //1
        glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS,
                &maxComputeTextureImageUnits); //1
        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS,
                &maxComputeUniformComponents); //1
        glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &maxComputeAtomicCounters); //1
        glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS,
                &maxComputeAtomicCounterBuffers); //1
        glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS,
                &maxCombinedComputeUniformComponents); //1
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS,
                &maxComputeWorkGroupInvocations); //1

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0,
                &(maxComputeWorkGroupCount[0])); //3
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1,
                &(maxComputeWorkGroupCount[1])); //3
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2,
                &(maxComputeWorkGroupCount[2])); //3

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0,
                &(maxComputeWorkGroupSize[0])); //3
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1,
                &(maxComputeWorkGroupSize[1])); //3
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2,
                &(maxComputeWorkGroupSize[2])); //3

        glGetIntegerv(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &maxDebugGroupStackDepth); //1
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion); //1
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max3DTextureSize); //1
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayTextureLayers); //1
        glGetIntegerv(GL_MAX_CLIP_DISTANCES, &maxClipDistances); //1
        glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &maxColorTextureSamples); //1
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments); //1
        glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTERS, &maxCombinedAtomicCounters); //1
        glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
                &maxCombinedFragmentUniformComponents); //1
        glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,
                &maxCombinedGeometryUniformComponents); //1
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                &maxCombinedTextureImageUnits); //1
        glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &maxCombinedUniformBlocks); //1
        glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
                &maxCombinedVertexUniformComponents); //1
        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxCubeMapTextureSize); //1
        glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &maxDepthTextureSamples); //1
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers); //1
        glGetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &maxDualSourceDrawBuffers); //1
        glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxElementsIndices); //1
        glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxElementsVertices); //1
        glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTERS, &maxFragmentAtomicCounters); //1
        glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS,
                &maxFragmentShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS,
                &maxFragmentInputComponents); //1
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
                &maxFragmentUniformComponents); //1
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniformVectors); //1
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks); //1
        glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth); //1
        glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight); //1
        glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &maxFramebufferLayers); //1
        glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &maxFramebufferSamples); //1
        glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTERS, &maxGeometryAtomicCounters); //1
        glGetIntegerv(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS,
                &maxGeometryShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS,
                &maxGeometryInputComponents); //1
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS,
                &maxGeometryOutputComponents); //1
        glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,
                &maxGeometryTextureImageUnits); //1
        glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &maxGeometryUniformBlocks); //1
        glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS,
                &maxGeometryUniformComponents); //1
        glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &maxIntegerSamples); //1
        glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &minMapBufferAlignment); //1
        glGetIntegerv(GL_MAX_LABEL_LENGTH, &maxLabelLength); //1
        glGetIntegerv(GL_MAX_PROGRAM_TEXEL_OFFSET, &maxProgramTexelOffset); //1
        glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET, &minProgramTexelOffset); //1
        glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &maxRectangleTextureSize); //1
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize); //1
        glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &maxSampleMaskWords); //1
        glGetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &maxServerWaitTimeout); //1
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
                &maxShaderStorageBufferBindings); //1
        glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS,
                &maxTessControlAtomicCounters); //1
        glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS,
                &maxTessEvaluationAtomicCounters); //1
        glGetIntegerv(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS,
                &maxTessControlShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS,
                &maxTessEvaluationShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTextureBufferSize); //1
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits); //1
        glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &maxTextureLODBias); //1
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize); //1
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBindings); //1
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize); //1
        glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations); //1
        glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &maxVaryingComponents); //1
        glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryingVectors); //1
        glGetIntegerv(GL_MAX_VARYING_FLOATS, &maxVaryingFloats); //1
        glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTERS, &maxVertexAtomicCounters); //1
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs); //1
        glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS,
                &maxVertexShaderStorageBlocks); //1
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                &maxVertexTextureImageUnits); //1
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,
                &maxVertexUniformComponents); //1
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniformVectors); //1
        glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &maxVertexOutputComponents); //1
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks); //1
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportDims); //2
        glGetIntegerv(GL_MAX_VIEWPORTS, &maxViewports); //1
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion); //1
        glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS,
                &numCompressedTextureFormats); //1
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions); //1
        glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &numProgramBinaryFormats); //1
        glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &numShaderBinaryFormats); //1
        glGetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET,
                &maxVertexAttribRelativeOffset); //1
        glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &maxVertexAttribBindings); //1
        glGetIntegerv(GL_MAX_ELEMENT_INDEX, &maxElementIndex); //1

        // at this point there may be errors due to unrecognized enums, pull
        // them off the queue.
        GLuint errnum = glGetError();
        while (errnum != GL_NO_ERROR)
        {
            errnum = glGetError();
        }
    }


    void print()
    {
        std::cout << "OpenGL " << majorVersion << "." << minorVersion;
        std::cout << std::endl << "MaxComputeShaderStorageBlocks "
                << maxComputeShaderStorageBlocks;
        std::cout << std::endl << "MaxCombinedShaderStorageBlocks "
                << maxCombinedShaderStorageBlocks;
        std::cout << std::endl << "MaxComputeUniformBlocks " << maxComputeUniformBlocks;
        std::cout << std::endl << "MaxComputeTextureImageUnits "
                << maxComputeTextureImageUnits;
        std::cout << std::endl << "MaxComputeUniformComponents "
                << maxComputeUniformComponents;
        std::cout << std::endl << "MaxComputeAtomicCounters "
                << maxComputeUniformComponents;
        std::cout << std::endl << "MaxComputeAtomicCounterBuffers "
                << maxComputeAtomicCounterBuffers;
        std::cout << std::endl << "MaxCombinedComputeUniformComponents "
                << maxCombinedComputeUniformComponents;
        std::cout << std::endl << "MaxComputeWorkGroupInvocations "
                << maxComputeWorkGroupInvocations;
        std::cout << std::endl << "MaxComputeWorkGroupCount[3] "
                << maxComputeWorkGroupCount[0] << " " << maxComputeWorkGroupCount[1]
                << " " << maxComputeWorkGroupCount[2];
        std::cout << std::endl << "MaxComputeWorkGroupSize[3] "
                << maxComputeWorkGroupSize[0] << " " << maxComputeWorkGroupSize[1]
                << " " << maxComputeWorkGroupSize[2];
        std::cout << std::endl << "MaxDebugGroupStackDepth " << maxDebugGroupStackDepth;

        std::cout << std::endl << "Max3DTextureSize " << max3DTextureSize;
        std::cout << std::endl << "MaxArrayTextureLayers " << maxArrayTextureLayers;
        std::cout << std::endl << "MaxClipDistances " << maxClipDistances;
        std::cout << std::endl << "MaxColorTextureSamples " << maxColorTextureSamples;
        std::cout << std::endl << "MaxColorTextureSamples " << maxColorAttachments;
        std::cout << std::endl << "MaxCombinedAtomicCounters "
                << maxCombinedAtomicCounters;
        std::cout << std::endl << "MaxCombinedFragmentUniformComponents "
                << maxCombinedFragmentUniformComponents;
        std::cout << std::endl << "MaxCombinedGeometryUniformComponents "
                << maxCombinedGeometryUniformComponents;
        std::cout << std::endl << "MaxCombinedTextureImageUnits "
                << maxCombinedTextureImageUnits;
        std::cout << std::endl << "MaxCombinedUniformBlocks " << maxCombinedUniformBlocks;
        std::cout << std::endl << "MaxCombinedVertexUniformComponents "
                << maxCombinedVertexUniformComponents;
        std::cout << std::endl << "MaxCubeMapTextureSize " << maxCubeMapTextureSize;
        std::cout << std::endl << "MaxDepthTextureSamples " << maxDepthTextureSamples;
        std::cout << std::endl << "MaxDrawBuffers " << maxDrawBuffers;
        std::cout << std::endl << "MaxDualSourceDrawBuffers " << maxDualSourceDrawBuffers;
        std::cout << std::endl << "MaxElementsIndices " << maxElementsIndices;
        std::cout << std::endl << "MaxElementsVertices " << maxElementsVertices;
        std::cout << std::endl << "MaxFragmentAtomicCounters "
                << maxFragmentAtomicCounters;
        std::cout << std::endl << "MaxFragmentShaderStorageBlocks "
                << maxFragmentShaderStorageBlocks;
        std::cout << std::endl << "MaxFragmentInputComponents "
                << maxFragmentInputComponents;
        std::cout << std::endl << "MaxFragmentUniformComponents "
                << maxFragmentUniformComponents;
        std::cout << std::endl << "MaxFragmentUniformVectors "
                << maxFragmentUniformVectors;
        std::cout << std::endl << "MaxFragmentUniformBlocks " << maxFragmentUniformBlocks;
        std::cout << std::endl << "MaxFramebufferWidth " << maxFramebufferWidth;
        std::cout << std::endl << "MaxFramebufferHeight " << maxFramebufferHeight;
        std::cout << std::endl << "MaxFramebufferLayers " << maxFramebufferLayers;
        std::cout << std::endl << "MaxFramebufferSamples " << maxFramebufferSamples;
        std::cout << std::endl << "MaxGeometryAtomicCounters "
                << maxGeometryAtomicCounters;
        std::cout << std::endl << "MaxGeometryShaderStorageBlocks "
                << maxGeometryShaderStorageBlocks;
        std::cout << std::endl << "MaxGeometryInputComponents "
                << maxGeometryInputComponents;
        std::cout << std::endl << "MaxGeometryOutputComponents "
                << maxGeometryOutputComponents;
        std::cout << std::endl << "MaxGeometryTextureImageUnits "
                << maxGeometryTextureImageUnits;
        std::cout << std::endl << "MaxGeometryUniformBlocks " << maxGeometryUniformBlocks;
        std::cout << std::endl << "MaxGeometryUniformComponents "
                << maxGeometryUniformComponents;
        std::cout << std::endl << "MaxIntegerSamples " << maxIntegerSamples;
        std::cout << std::endl << "MinMapBufferAlignment " << minMapBufferAlignment;
        std::cout << std::endl << "MaxLabelLength " << maxLabelLength;
        std::cout << std::endl << "MaxProgramTexelOffset " << maxProgramTexelOffset;
        std::cout << std::endl << "MinProgramTexelOffset " << minProgramTexelOffset;
        std::cout << std::endl << "MaxRectangleTextureSize " << maxRectangleTextureSize;
        std::cout << std::endl << "MaxRenderbufferSize " << maxRenderbufferSize;
        std::cout << std::endl << "MaxSampleMaskWords " << maxSampleMaskWords;
        std::cout << std::endl << "MaxServerWaitTimeout " << maxServerWaitTimeout;
        std::cout << std::endl << "MaxShaderStorageBufferBindings "
                << maxShaderStorageBufferBindings;
        std::cout << std::endl << "MaxTessControlAtomicCounters "
                << maxTessControlAtomicCounters;
        std::cout << std::endl << "MaxTessEvaluationAtomicCounters "
                << maxTessEvaluationAtomicCounters;
        std::cout << std::endl << "MaxTessControlShaderStorageBlocks "
                << maxTessControlShaderStorageBlocks;
        std::cout << std::endl << "MaxTessEvaluationShaderStorageBlocks "
                << maxTessEvaluationShaderStorageBlocks;
        std::cout << std::endl << "MaxTextureBufferSize " << maxTextureBufferSize;
        std::cout << std::endl << "MaxTextureImageUnits " << maxTextureImageUnits;
        std::cout << std::endl << "MaxTextureLODBias " << maxTextureLODBias;
        std::cout << std::endl << "MaxTextureSize " << maxTextureSize;
        std::cout << std::endl << "MaxUniformBufferBindings " << maxUniformBufferBindings;
        std::cout << std::endl << "MaxUniformBlockSize " << maxUniformBlockSize;
        std::cout << std::endl << "MaxUniformLocations " << maxUniformLocations;
        std::cout << std::endl << "MaxVaryingComponents " << maxVaryingComponents;
        std::cout << std::endl << "MaxVaryingVectors " << maxVaryingVectors;
        std::cout << std::endl << "MaxVaryingFloats " << maxVaryingFloats;
        std::cout << std::endl << "MaxVertexAtomicCounters " << maxVertexAtomicCounters;
        std::cout << std::endl << "MaxVertexAttribs " << maxVertexAttribs;
        std::cout << std::endl << "MaxVertexShaderStorageBlocks "
                << maxVertexShaderStorageBlocks;
        std::cout << std::endl << "MaxVertexTextureImageUnits "
                << maxVertexTextureImageUnits;
        std::cout << std::endl << "MaxVertexUniformComponents "
                << maxVertexUniformComponents;
        std::cout << std::endl << "MaxVertexUniformVectors " << maxVertexUniformVectors;
        std::cout << std::endl << "MaxVertexOutputComponents "
                << maxVertexOutputComponents;
        std::cout << std::endl << "MaxVertexUniformBlocks " << maxVertexUniformBlocks;
        std::cout << std::endl << "MaxViewportDims[2] " << maxViewportDims[0] << " "
                << maxViewportDims[1];
        std::cout << std::endl << "MaxViewports " << maxViewports;
        std::cout << std::endl << "NumCompreoutedTextureFormats "
                << numCompressedTextureFormats;
        std::cout << std::endl << "NumExtensions " << numExtensions;
        std::cout << std::endl << "NumProgramBinaryFormats " << numProgramBinaryFormats;
        std::cout << std::endl << "NumShaderBinaryFormats " << numShaderBinaryFormats;
        std::cout << std::endl << "MaxVertexAttribRelativeOffset "
                << maxVertexAttribRelativeOffset;
        std::cout << std::endl << "MaxVertexAttribBindings " << maxVertexAttribBindings;
        std::cout << std::endl << "MaxElementIndex " << maxElementIndex;
        std::cout << std::endl;
    }


    std::string _errorToString(GLenum errorCode)
    {
        switch (errorCode)
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


    void reportError(const std::string &str)
    {
        uint32_t numErrs = 0;
        GLuint errnum = glGetError();
        while (errnum != GL_NO_ERROR)
        {
            std::cout << str << " " << _errorToString(errnum) << std::endl;
            numErrs++;
            errnum = glGetError();
        }
    }


    uint32_t sizeOfType(uint32_t glEnum)
    {
        switch(glEnum)
        {
            case GL_UNSIGNED_BYTE:      return sizeof(GLubyte);
            case GL_BYTE:               return sizeof(GLbyte);
            case GL_UNSIGNED_SHORT:     return sizeof(GLushort);
            case GL_SHORT:              return sizeof(GLshort);
            case GL_UNSIGNED_INT:       return sizeof(GLuint);
            case GL_INT:                return sizeof(GLint);
            case GL_FLOAT:              return sizeof(GLfloat);
            case GL_DOUBLE:             return sizeof(GLdouble);
        }
        return 0;
    }
}
