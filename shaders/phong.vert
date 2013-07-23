#version 330

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
    if (cColour == vec4(0,0,0,0))
        cColourInterp = vColour;
    else
        cColourInterp = cColour;
    
	mat4 modelView = mView * mModel;

    // view space normal 
    vViewNormal = mat3(modelView) * vNormal; 
     
    //  vector to light source              
    vViewLightDirection = vLightPosition - vec3(modelView * vec4(vPosition,1));  
    
    gl_Position = mProjection * mView * mModel * vec4(vPosition,1);
}

