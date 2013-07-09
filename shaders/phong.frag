#version 120

varying vec3 vViewNormal;
varying vec3 vViewLightDirection;

void main(void)
{ 

    vec3 N = normalize(vViewNormal);
    vec3 L = normalize(vViewLightDirection);
    vec3 R = normalize(reflect(-L, N));   
    
    float diff = abs(dot(N,L));
    float spec = pow(abs( dot(N, R)), 128);
    
    // Multiply intensity by diffuse color, force alpha to 1.0
    gl_FragColor = vec4(gl_Color.r*diff, gl_Color.g*diff, gl_Color.b*diff, gl_Color.a)
                 + vec4(gl_Color.r*0.1,  gl_Color.b*0.1,  gl_Color.g*0.1,  0)
                 + vec4(spec, spec, spec, 0);
    
}
