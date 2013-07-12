#version 330

uniform vec4 cColour;

out vec4 cFragColour;

void main()
{ 
	cFragColour = cColour;
}
