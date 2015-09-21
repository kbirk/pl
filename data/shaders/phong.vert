#version 330

#define PL_COLOUR_MESH_OPAQUE_COLOUR             0,  0,  0, 0
#define PL_COLOUR_MESH_TRANSPARENT_COLOUR       -1, -1, -1, 0

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColour;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec4 uColour;
uniform vec3 uLightPosition;

out vec4 colourInterp;
out vec3 viewNormal;
out vec3 viewLightDirection;


void main()
{
    // if vertex colour attribute is unspecified, all indices are 1
    // if vertex colour is unspecified, use uniform, else use vertex colour (for colour meshes) 
   
    if ( uColour == vec4( PL_COLOUR_MESH_OPAQUE_COLOUR ) )
    {
        colourInterp = vColour;
    }    
    else if ( uColour == vec4( PL_COLOUR_MESH_TRANSPARENT_COLOUR ) )
    {
        colourInterp = vec4( vColour.x, vColour.y, vColour.z, 0.7 );
    }  
    else
        colourInterp = uColour;
    
	mat4 modelView = uViewMatrix * uModelMatrix;

    // view space normal 
    viewNormal = mat3( modelView ) * vNormal; 
     
    //  vector to light source              
    viewLightDirection = uLightPosition - vec3( modelView * vec4 (vPosition,1 ));  

    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4( vPosition, 1 ); 
}

