#version 330

in vec2 texCoordOut;

uniform sampler2D uTextureUnit0;

layout( location = 0 ) out vec4  dummy0;     // GL_COLOR_ATTACHMENT0
layout( location = 1 ) out vec4  dummy1;     // GL_COLOR_ATTACHMENT1
layout( location = 2 ) out vec4  colour; 	 // GL_COLOR_ATTACHMENT2

void main()
{
    // read texture from sampler2D
    colour = texture2D( uTextureUnit0, vec2( texCoordOut.x, texCoordOut.y ) ); // openGL texture rows start at bottem, hence negative y
}
