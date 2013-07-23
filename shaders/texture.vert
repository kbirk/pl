#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 3) in vec2 vTexCoord;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

out vec2 vTexCoordOut;

void main()
{
	// project position into projection space	
	gl_Position = mProjection * mView * mModel * vec4(vPosition, 1.0);

    // pass texture coordinate 
	vTexCoordOut = vTexCoord;
}
