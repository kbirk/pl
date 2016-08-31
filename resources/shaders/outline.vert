#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
    // NOTE: the EXACT order of calculation must match that of the `phong`
    // shader otherwise the GL_LEQUAL depth test for the outline will cause
    // z-fighting.
    mat4 modelView = uViewMatrix * uModelMatrix;
    gl_Position = uProjectionMatrix * modelView * vec4(aPosition, 1.0f);
}
