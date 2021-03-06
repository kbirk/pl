

// functions

void setDefectCapTriangleIndex(in uint graftIndex, in uint i, in uint triIndex);
void setDefectCapIndexCount(in uint graftIndex, in uint count);
void setDonorCapTriangleIndex(in uint i, in uint triIndex);
void setDonorCapIndexCount(in uint count);

uint getDefectCapCount(in uint graftIndex);
uint getDonorCapCount();

triangle getDefectCapTriangles(in uint graftIndex, in vec3 graftPosition, in uint index);
triangle getDonorCapTriangles(in uint site, in vec3 gridPosition, in uint index);
