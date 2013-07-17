#version 330

in vec3 vPosition;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

void main()
{
	gl_Position = mProjection * mView * mModel * vec4(vPosition,1);
}
