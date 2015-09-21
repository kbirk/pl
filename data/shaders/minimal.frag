#version 330

layout( location = 0 ) out vec4   colourOutput;    // GL_COLOR_ATTACHMENT0
layout( location = 1 ) out ivec4  dummy0;          // GL_COLOR_ATTACHMENT1 // outline
layout( location = 2 ) out vec4   dummy1;          // GL_COLOR_ATTACHMENT2
layout( location = 3 ) out vec4   dummy2;      	   // GL_COLOR_ATTACHMENT3 // arthro
layout( location = 4 ) out ivec3  pickingOutput;   // GL_COLOR_ATTACHMENT4

uniform vec4 uColour;
uniform ivec3 uPickingColour;

void main()
{
    colourOutput  = uColour;
    pickingOutput = uPickingColour;   
}