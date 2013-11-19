#version 330

in vec2 texCoordOut;

uniform sampler2D uTextureUnit0;

out vec4 colour;

void main()
{
    // read texture from sampler2D
    colour = texture2D( uTextureUnit0, vec2( texCoordOut.x, texCoordOut.y ) ); // openGL texture rows start at bottem, hence negative y
}
