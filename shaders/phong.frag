#version 330

in vec4 cColourInterp;
in vec3 vViewNormal;
in vec3 vViewLightDirection;

out vec4 cFragColour;

void main()
{
	vec3 N = normalize(vViewNormal);
    vec3 L = normalize(vViewLightDirection);
    vec3 R = normalize(reflect(-L, N));   
    
    float diff = abs(dot(N,L));
    float spec = pow(abs( dot(N, R)), 128);
    
    // Multiply intensity by diffuse color, force alpha to 1.0
    cFragColour =  vec4(cColourInterp.r*diff, cColourInterp.g*diff, cColourInterp.b*diff, cColourInterp.a)
                 + vec4(cColourInterp.r*0.1,  cColourInterp.b*0.1,  cColourInterp.g*0.1,  0)
                 + vec4(spec, spec, spec, 0);   
}
