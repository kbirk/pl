#pragma once

#include "plCommon.h"
#include "plLog.h"
#include "plOpenGLInfo.h"

/**
 * Enable OpenGL logging by default
 */
#define LOGGING_ENABLED 1

/**
 * OpenGL logging macro
 */
#ifdef LOGGING_ENABLED
#define LOG_OPENGL(x)                                                  \
    {                                                                  \
        GLuint error = glGetError();                                   \
        if (error != GL_NO_ERROR) {                                    \
            LOG_ERROR(x << " " << plOpenGLInfo::errorToString(error)); \
        }                                                              \
    }
#else
#define LOG_OPENGL(x)
#endif
