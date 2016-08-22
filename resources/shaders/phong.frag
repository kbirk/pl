#version 330

in vec4 vColor;
in vec3 vViewNormal;
in vec3 vViewLightDirection;

layout(location = 0) out vec4   colorOutput;         // GL_COLOR_ATTACHMENT0
layout(location = 1) out ivec4  dummy0;              // GL_COLOR_ATTACHMENT1
layout(location = 2) out vec4   dummy1;              // GL_COLOR_ATTACHMENT2
layout(location = 3) out vec4   dummy2;              // GL_COLOR_ATTACHMENT3
layout(location = 4) out ivec3  pickingOutput;       // GL_COLOR_ATTACHMENT4

uniform ivec3 uPickingColor;

void main()
{
    vec3 N = normalize(vViewNormal);
    vec3 L = normalize(vViewLightDirection);
    vec3 R = normalize(reflect(-L, N));

    float diff = abs(dot(N,L));
    float spec = pow(abs(dot(N, R)), 128);

    vec3 ambient = vec3(vColor.rgb * 0.1);
    vec3 diffuse = vec3(vColor.rgb * diff);
    vec3 specular = vec3(spec, spec, spec);

    colorOutput = vec4(ambient + diffuse + specular, vColor.a);
    pickingOutput = uPickingColor;
}
