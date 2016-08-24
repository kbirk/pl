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
        LOG_INFO("OpenGL Version: " << majorVersion << "." << minorVersion);
        LOG_INFO("MaxComputeShaderStorageBlocks: " << maxComputeShaderStorageBlocks);
        LOG_INFO("MaxCombinedShaderStorageBlocks: " << maxCombinedShaderStorageBlocks);
        LOG_INFO("MaxComputeUniformBlocks: " << maxComputeUniformBlocks);
        LOG_INFO("MaxComputeTextureImageUnits: " << maxComputeTextureImageUnits);
        LOG_INFO("MaxComputeUniformComponents: " << maxComputeUniformComponents);
        LOG_INFO("MaxComputeAtomicCounters: " << maxComputeUniformComponents);
        LOG_INFO("MaxComputeAtomicCounterBuffers: " << maxComputeAtomicCounterBuffers);
        LOG_INFO("MaxCombinedComputeUniformComponents: " << maxCombinedComputeUniformComponents);
        LOG_INFO("MaxComputeWorkGroupInvocations: " << maxComputeWorkGroupInvocations);
        LOG_INFO("MaxComputeWorkGroupCount[3]: " << maxComputeWorkGroupCount[0] << " " << maxComputeWorkGroupCount[1] << " " << maxComputeWorkGroupCount[2]);
        LOG_INFO("MaxComputeWorkGroupSize[3]: " << maxComputeWorkGroupSize[0] << " " << maxComputeWorkGroupSize[1] << " " << maxComputeWorkGroupSize[2]);
        LOG_INFO("MaxDebugGroupStackDepth: " << maxDebugGroupStackDepth);
        LOG_INFO("Max3DTextureSize: " << max3DTextureSize);
        LOG_INFO("MaxArrayTextureLayers: " << maxArrayTextureLayers);
        LOG_INFO("MaxClipDistances: " << maxClipDistances);
        LOG_INFO("MaxColorTextureSamples: " << maxColorTextureSamples);
        LOG_INFO("MaxColorTextureSamples: " << maxColorAttachments);
        LOG_INFO("MaxCombinedAtomicCounters: " << maxCombinedAtomicCounters);
        LOG_INFO("MaxCombinedFragmentUniformComponents: " << maxCombinedFragmentUniformComponents);
        LOG_INFO("MaxCombinedGeometryUniformComponents: " << maxCombinedGeometryUniformComponents);
        LOG_INFO("MaxCombinedTextureImageUnits: " << maxCombinedTextureImageUnits);
        LOG_INFO("MaxCombinedUniformBlocks: " << maxCombinedUniformBlocks);
        LOG_INFO("MaxCombinedVertexUniformComponents: " << maxCombinedVertexUniformComponents);
        LOG_INFO("MaxCubeMapTextureSize: " << maxCubeMapTextureSize);
        LOG_INFO("MaxDepthTextureSamples: " << maxDepthTextureSamples);
        LOG_INFO("MaxDrawBuffers: " << maxDrawBuffers);
        LOG_INFO("MaxDualSourceDrawBuffers: " << maxDualSourceDrawBuffers);
        LOG_INFO("MaxElementsIndices: " << maxElementsIndices);
        LOG_INFO("MaxElementsVertices: " << maxElementsVertices);
        LOG_INFO("MaxFragmentAtomicCounters: " << maxFragmentAtomicCounters);
        LOG_INFO("MaxFragmentShaderStorageBlocks: " << maxFragmentShaderStorageBlocks);
        LOG_INFO("MaxFragmentInputComponents: " << maxFragmentInputComponents);
        LOG_INFO("MaxFragmentUniformComponents: " << maxFragmentUniformComponents);
        LOG_INFO("MaxFragmentUniformVectors: " << maxFragmentUniformVectors);
        LOG_INFO("MaxFragmentUniformBlocks: " << maxFragmentUniformBlocks);
        LOG_INFO("MaxFramebufferWidth: " << maxFramebufferWidth);
        LOG_INFO("MaxFramebufferHeight: " << maxFramebufferHeight);
        LOG_INFO("MaxFramebufferLayers: " << maxFramebufferLayers);
        LOG_INFO("MaxFramebufferSamples: " << maxFramebufferSamples);
        LOG_INFO("MaxGeometryAtomicCounters: " << maxGeometryAtomicCounters);
        LOG_INFO("MaxGeometryShaderStorageBlocks: " << maxGeometryShaderStorageBlocks);
        LOG_INFO("MaxGeometryInputComponents: " << maxGeometryInputComponents);
        LOG_INFO("MaxGeometryOutputComponents: " << maxGeometryOutputComponents);
        LOG_INFO("MaxGeometryTextureImageUnits: " << maxGeometryTextureImageUnits);
        LOG_INFO("MaxGeometryUniformBlocks: " << maxGeometryUniformBlocks);
        LOG_INFO("MaxGeometryUniformComponents: " << maxGeometryUniformComponents);
        LOG_INFO("MaxIntegerSamples: " << maxIntegerSamples);
        LOG_INFO("MinMapBufferAlignment: " << minMapBufferAlignment);
        LOG_INFO("MaxLabelLength: " << maxLabelLength);
        LOG_INFO("MaxProgramTexelOffset: " << maxProgramTexelOffset);
        LOG_INFO("MinProgramTexelOffset: " << minProgramTexelOffset);
        LOG_INFO("MaxRectangleTextureSize: " << maxRectangleTextureSize);
        LOG_INFO("MaxRenderbufferSize: " << maxRenderbufferSize);
        LOG_INFO("MaxSampleMaskWords: " << maxSampleMaskWords);
        LOG_INFO("MaxServerWaitTimeout: " << maxServerWaitTimeout);
        LOG_INFO("MaxShaderStorageBufferBindings: " << maxShaderStorageBufferBindings);
        LOG_INFO("MaxTessControlAtomicCounters: " << maxTessControlAtomicCounters);
        LOG_INFO("MaxTessEvaluationAtomicCounters: " << maxTessEvaluationAtomicCounters);
        LOG_INFO("MaxTessControlShaderStorageBlocks: " << maxTessControlShaderStorageBlocks);
        LOG_INFO("MaxTessEvaluationShaderStorageBlocks: " << maxTessEvaluationShaderStorageBlocks);
        LOG_INFO("MaxTextureBufferSize: " << maxTextureBufferSize);
        LOG_INFO("MaxTextureImageUnits: " << maxTextureImageUnits);
        LOG_INFO("MaxTextureLODBias: " << maxTextureLODBias);
        LOG_INFO("MaxTextureSize: " << maxTextureSize);
        LOG_INFO("MaxUniformBufferBindings: " << maxUniformBufferBindings);
        LOG_INFO("MaxUniformBlockSize: " << maxUniformBlockSize);
        LOG_INFO("MaxUniformLocations: " << maxUniformLocations);
        LOG_INFO("MaxVaryingComponents: " << maxVaryingComponents);
        LOG_INFO("MaxVaryingVectors: " << maxVaryingVectors);
        LOG_INFO("MaxVaryingFloats: " << maxVaryingFloats);
        LOG_INFO("MaxVertexAtomicCounters: " << maxVertexAtomicCounters);
        LOG_INFO("MaxVertexAttribs: " << maxVertexAttribs);
        LOG_INFO("MaxVertexShaderStorageBlocks: " << maxVertexShaderStorageBlocks);
        LOG_INFO("MaxVertexTextureImageUnits: " << maxVertexTextureImageUnits);
        LOG_INFO("MaxVertexUniformComponents: " << maxVertexUniformComponents);
        LOG_INFO("MaxVertexUniformVectors: " << maxVertexUniformVectors);
        LOG_INFO("MaxVertexOutputComponents: " << maxVertexOutputComponents);
        LOG_INFO("MaxVertexUniformBlocks: " << maxVertexUniformBlocks);
        LOG_INFO("MaxViewportDims[2]: " << maxViewportDims[0] << " " << maxViewportDims[1]);
        LOG_INFO("MaxViewports: " << maxViewports);
        LOG_INFO("NumCompreoutedTextureFormats: " << numCompressedTextureFormats);
        LOG_INFO("NumExtensions: " << numExtensions);
        LOG_INFO("NumProgramBinaryFormats: " << numProgramBinaryFormats);
        LOG_INFO("NumShaderBinaryFormats: " << numShaderBinaryFormats);
        LOG_INFO("MaxVertexAttribRelativeOffset: " << maxVertexAttribRelativeOffset);
        LOG_INFO("MaxVertexAttribBindings: " << maxVertexAttribBindings);
        LOG_INFO("MaxElementIndex: " << maxElementIndex);
    }

    std::string errorToString(GLenum errorCode) {
		switch (errorCode) {
			case GL_NO_ERROR:
				return "No error has been recorded.";
			case GL_INVALID_ENUM:
				return "GL_INVALID_ENUM: An unacceptable value is specified "
					"for an enumerated argument. The offending command is "
					"ignored and has no other side effect than to set the "
					"error flag.";
			case GL_INVALID_VALUE:
				return "GL_INVALID_VALUE: A numeric argument is out of range. "
					"The offending command is ignored and has no other side "
					"effect than to set the error flag.";
			case GL_INVALID_OPERATION:
				return "GL_INVALID_OPERATION: The specified operation is not "
					"allowed in the current state. The offending command is "
					"ignored and has no other side effect than to set the "
					"error flag.";
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer "
					"object is not complete. The offending command is ignored "
					"and has no other side effect than to set the error flag.";
			case GL_OUT_OF_MEMORY:
				return "GL_OUT_OF_MEMORY: There is not enough memory left to "
					"execute the command. The state of the GL is undefined, "
					"except for the state of the error flags, after this error "
					"is recorded.";
			case GL_STACK_UNDERFLOW:
				return "GL_STACK_UNDERFLOW: An attempt has been made to "
					"perform an operation that would cause an internal stack "
					"to underflow.";
			case GL_STACK_OVERFLOW:
				return "GL_STACK_OVERFLOW: An attempt has been made to perform "
					"an operation that would cause an internal stack to "
					"overflow.";
		}
		return "UNRECOGNIZED_ERROR: The error code is unrecognized";
	}
}
