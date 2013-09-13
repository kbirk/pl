#version 330

#define PL_COLOUR_MESH_OPAQUE_COLOUR             0,  0,  0, 0
#define PL_COLOUR_MESH_TRANSPARENT_COLOUR       -1, -1, -1, 0


layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColour;

uniform vec3 vLightPosition;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform vec4 cColour;

out vec4 cColourInterp;
out vec3 vViewNormal;
out vec3 vViewLightDirection;

void main()
{
    // if vertex colour attribute is unspecified, all indices are 1
    // if vertex colour is unspecified, use uniform, else use vertex colour (for colour meshes)       
    if ( cColour == vec4( PL_COLOUR_MESH_OPAQUE_COLOUR ) )
    {
        cColourInterp = vColour;
    }    
    else if ( cColour == vec4( PL_COLOUR_MESH_TRANSPARENT_COLOUR ) )
    {
        cColourInterp = vec4( vColour.x, vColour.y, vColour.z, 0.2 );
    }  
    else
        cColourInterp = cColour;
    
	mat4 modelView = mView * mModel;

    // view space normal 
    vViewNormal = mat3(modelView) * vNormal; 
     
    //  vector to light source              
    vViewLightDirection = vLightPosition - vec3(modelView * vec4(vPosition,1));  
    
    gl_Position = mProjection * mView * mModel * vec4(vPosition,1);
}

