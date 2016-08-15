
layout (local_size_x = PL_STAGE_1_WORKGROUP_SIZE) in;

// input data
layout (std430,  binding = 0) buffer defectSiteDataBlock    { vec4 defectSiteData[]; };
layout (std430,  binding = 1) buffer donorSiteDataBlock     { vec4 donorSiteData []; };

// output data
layout (std430,  binding = 2) buffer defectCapIndicesBlock  { uint defectCapIndices[]; };
layout (std430,  binding = 3) buffer donorCapIndicesBlock   { uint donorCapIndices[];  };
