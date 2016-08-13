#version 330

in vec4 colourInterp;
in vec3 viewNormal;
in vec3 viewLightDirection;

layout(location = 0) out vec4   colourOutput;        // GL_COLOR_ATTACHMENT0
layout(location = 1) out ivec4  dummy0;              // GL_COLOR_ATTACHMENT1 // outline
layout(location = 2) out vec4   transparencyOutput;  // GL_COLOR_ATTACHMENT2
layout(location = 3) out vec4   dummy2;              // GL_COLOR_ATTACHMENT3
layout(location = 4) out ivec3  pickingOutput;       // GL_COLOR_ATTACHMENT4

uniform ivec3 uPickingColour;

void main()
{
    vec3 N = normalize(viewNormal);
    vec3 L = normalize(viewLightDirection);
    vec3 R = normalize(reflect(-L, N));

    float32_t diff = abs(dot(N,L));
    float32_t spec = pow(abs(dot(N, R)), 128);

    // Multiply intensity by diffuse color, force alpha to 1.0
    vec4 colour = vec4(colourInterp.r*diff, colourInterp.g*diff, colourInterp.b*diff, colourInterp.a)
                + vec4(colourInterp.r*0.1,  colourInterp.b*0.1,  colourInterp.g*0.1,  0.0)
                + vec4(spec,                spec,                spec,                0.0);

    colourOutput = colour;

    transparencyOutput = colour;

    pickingOutput = uPickingColour;
}
