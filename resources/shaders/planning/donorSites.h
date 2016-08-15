
// uniforms

uniform uint  uDonorSiteCount;
uniform uint  uDonorTotalGridPointCount;
uniform uint  uDonorSiteTriangleCounts      [PL_MAX_DONOR_SITES];
uniform uint  uDonorSiteGridPointCounts     [PL_MAX_DONOR_SITES];
uniform uint  uDonorSiteBoundaryPointCounts [PL_MAX_DONOR_SITES];
uniform uint  uDonorSiteDataOffsets         [PL_MAX_DONOR_SITES];


// functions

void getDonorSiteAndIndex(inout uint site, inout uint localIndex);    // uses gl_GlobalInvocationID
void getDonorSiteAndIndex(inout uint site, inout uint localIndex, in uint globalIndex);
vec4 donorSitePositions(in uint site, in uint i);
vec4 donorSiteNormals(in uint site, in uint i);
triangle donorSiteTriangles(in uint site, in uint i);
edge donorSiteEdge(in uint site, in uint i);

bool isWithinDonorBoundary(in uint site, in vec3 position, in vec3 normal, in float radius);
