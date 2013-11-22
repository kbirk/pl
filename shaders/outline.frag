#version 330

layout( location = 0 ) out vec4  dummy;          // GL_COLOR_ATTACHMENT0
layout( location = 1 ) out vec4  outlineOutput;  // GL_COLOR_ATTACHMENT1


void main()
{
    outlineOutput = vec4( 1, 1, 1, 1 ); 
}
