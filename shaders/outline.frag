#version 330

layout( location = 0 ) out vec4  dummy;          // GL_COLOR_ATTACHMENT0
layout( location = 1 ) out ivec4 outlineOutput;  // GL_COLOR_ATTACHMENT1

uniform ivec3 uPickingColour;

void main()
{
    outlineOutput = ivec4( uPickingColour, 1 ); 
}
