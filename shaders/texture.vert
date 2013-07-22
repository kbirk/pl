#version 330

in vec3 vPosition;
in vec2 vTexCoord;

out vec2 vTexCoordOut;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

void main()
{
	// project position into projection space	
	gl_Position = mProjection * mView * mModel * vec4(vPosition, 1.0);

    // pass texture coordinate 
	vTexCoordOut = vTexCoord;
}
