#version 330 
//#extension GL_EXT_gpu_shader4 : enable 

out ivec3 fragColour;

uniform int uRedBits;
uniform int uGreenBits;
uniform int uBlueBits;

void main()
{
    fragColour = ivec3(uRedBits, uGreenBits, uBlueBits);
    //gl_FragColor = vec4(uRedBits, uGreenBits, uBlueBits, 1); 
}
