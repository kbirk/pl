#version 120

uniform vec3   vLightPosition;

varying vec3 vViewNormal;
varying vec3 vViewLightDirection;

void main(void) 
{ 	
    
    // view space normal 
    vViewNormal = mat3(gl_ModelViewMatrix) * gl_Normal;   
     
    //  vector to light source              
    vViewLightDirection = vLightPosition - vec3(gl_ModelViewMatrix * gl_Vertex);   
    
    gl_FrontColor = gl_Color;
        
    gl_Position = ftransform(); 

}

