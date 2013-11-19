#version 330

layout( location = 0 ) in vec3 vPosition;
layout( location = 3 ) in vec2 vTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 texCoordOut;

void main()
{
	// project position into projection space	
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4( vPosition, 1.0 );

    // pass texture coordinate 
	texCoordOut = vTexCoord;
}
