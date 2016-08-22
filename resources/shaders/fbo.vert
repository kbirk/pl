#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 vTexCoord;

void main()
{
    // project position into projection space
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
    // pass texture coordinate
    vTexCoord = aTexCoord;
}
