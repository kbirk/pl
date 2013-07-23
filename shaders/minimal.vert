#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

void main()
{   
	gl_Position = mProjection * mView * mModel * vec4(vPosition,1);
}

