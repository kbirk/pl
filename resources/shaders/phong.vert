#version 330

#define PL_USE_ATTRIBUTE_COLOR  0, 0, 0, 0

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec4 uColor;
uniform vec3 uLightPosition;

out vec4 vColor;
out vec3 vViewNormal;
out vec3 vViewLightDirection;

void main()
{
    // if uniform is set to vec4(PL_USE_ATTRIBUTE_COLOR), use the attribute colors instead
    if (uColor == vec4(PL_USE_ATTRIBUTE_COLOR))
    {
        // attribute color
        vColor = aColor;
    }
    else
    {
        // uniform color
        vColor = uColor;
    }
    // pre-compute modelview matrix
    mat4 modelView = uViewMatrix * uModelMatrix;
    // get vec4 of position
    vec4 position = vec4(aPosition, 1.0f);
    // view space normal
    vViewNormal = mat3(modelView) * aNormal;
    //  vector to light source
    vViewLightDirection = uLightPosition - vec3(modelView * position);
    // NOTE: the EXACT order of calculation must match that of the `outline`
    // shader otherwise the GL_LEQUAL depth test for the outline will cause
    // z-fighting.
    // write out position
    gl_Position = uProjectionMatrix * modelView * position;
}
