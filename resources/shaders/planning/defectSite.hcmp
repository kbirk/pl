

// uniforms

uniform uint  uDefectSiteGridPointCount;
uniform uint  uDefectSiteTriangleCount;
uniform uint  uDefectSiteBoundaryPointCount;
uniform vec4  uDefectSiteAvgNormal;
uniform float uDefectSiteArea;

// functions

vec4 defectSitePositions(in uint i);
vec4 defectSiteNormals(in uint i);
triangle defectSiteTriangles(in uint i);
edge defectSiteEdge(in uint i);

bool isWithinDefectBoundaryThreshold(in vec3 position, in vec3 normal, in float threshold);
bool isWithinDefectRotationThreshold(in vec3 normal, in float threshold);
