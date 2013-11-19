#version 330 

out ivec3 fragColour;

uniform int uRedBits;
uniform int uGreenBits;
uniform int uBlueBits;

void main()
{
    fragColour = ivec3( uRedBits, uGreenBits, uBlueBits );
}
