

// uniforms

uniform uint  uDefectSolutionGraftCount;
uniform vec4  uDefectSolutionGraftPositions [PL_MAX_GRAFTS_PER_SOLUTION];
uniform vec4  uDefectSolutionGraftNormals   [PL_MAX_GRAFTS_PER_SOLUTION];
uniform float uDefectSolutionGraftRadii     [PL_MAX_GRAFTS_PER_SOLUTION];
uniform vec4  uDefectSolutionSurfaceNormals [PL_MAX_GRAFTS_PER_SOLUTION];

// functions

uint defectSolutionGraftCount();
vec3 defectSolutionGraftPositions(in uint i);
vec3 defectSolutionGraftNormals(in uint i);
float defectSolutionGraftRadii(in uint i);
vec3 defectSolutionSurfaceNormals(in uint i);

void defectToDonorRotation(inout mat4 rotation, in vec3 defectSurfaceNormal, in vec3 donorSurfaceNormal, in float rotationAngle);
