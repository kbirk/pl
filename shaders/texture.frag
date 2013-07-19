#version 330

in vec2 vTexCoordOut;

out vec4 colour;

uniform sampler2D textureSampler;

void main()
{
    // read texture from sampler2D
    colour = texture2D(textureSampler, vec2(vTexCoordOut.x, -vTexCoordOut.y)); 
}
