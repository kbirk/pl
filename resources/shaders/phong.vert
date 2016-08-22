#version 330

#define PL_COLOR_MESH_OPAQUE_COLOR        0,  0,  0, 0
#define PL_COLOR_MESH_TRANSPARENT_COLOR  -1, -1, -1, 0

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
    // if vertex color attribute is unspecified, all indices are 1
    // if vertex color is unspecified, use uniform, else use vertex color (for color meshes)
    if (uColor == vec4(PL_COLOR_MESH_OPAQUE_COLOR))
    {
        vColor = aColor;
    }
    else if (uColor == vec4(PL_COLOR_MESH_TRANSPARENT_COLOR))
    {
        vColor = vec4(aColor.r, aColor.g, aColor.b, 0.7f);
    }
    else
    {
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
    // write out position
    gl_Position = uProjectionMatrix * modelView * position;
}
