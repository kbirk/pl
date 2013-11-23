#version 330

in vec4 colourInterp;
in vec3 viewNormal;
in vec3 viewLightDirection;

layout( location = 0 ) out vec4  opaqueOutput;       // GL_COLOR_ATTACHMENT0
layout( location = 1 ) out vec4  outlineOutput;      // GL_COLOR_ATTACHMENT1
layout( location = 2 ) out vec4  dummy;      	     // GL_COLOR_ATTACHMENT2
layout( location = 3 ) out ivec3 pickingOutput;      // GL_COLOR_ATTACHMENT3

uniform ivec3 uPickingColour;

uniform int  uOutlineEnabled; 

void main()
{
	vec3 N = normalize( viewNormal );
    vec3 L = normalize( viewLightDirection );
    vec3 R = normalize( reflect(-L, N) );   
    
    float diff = abs( dot( N,L ) );
    float spec = pow( abs( dot( N, R ) ), 128 );
    
    // Multiply intensity by diffuse color, force alpha to 1.0
    vec4 colourOut = vec4( colourInterp.r*diff, colourInterp.g*diff, colourInterp.b*diff, colourInterp.a )
                   + vec4( colourInterp.r*0.1,  colourInterp.b*0.1,  colourInterp.g*0.1,  0.0 )
                   + vec4( spec,                spec,                spec,                0.0 );   
             
    //outlineOutput = vec4( uOutlineEnabled, 1, 1, 1 );    
    opaqueOutput = colourOut;  
    pickingOutput = uPickingColour;
    
}
