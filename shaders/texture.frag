#version 330

in vec2 vTexCoordOut;

uniform sampler2D textureSampler;

out vec4 colour;

void main()
{
    // read texture from sampler2D
    colour = texture2D(textureSampler, vec2(vTexCoordOut.x, -vTexCoordOut.y)); // openGL texture rows start at bottem, hence negative y
}
