
layout (local_size_x = PL_STAGE_3_WORKGROUP_SIZE) in;

// input data
layout (std430,  binding = 0) buffer defectSiteDataBlock   { vec4 defectSiteData[]; };
layout (std430,  binding = 1) buffer donorSiteDataBlock    { vec4 donorSiteData []; };
layout (std430,  binding = 2) buffer rmsValuesBlock        { float rmsBuffer[];     };

// output
layout (std430,  binding = 3) buffer donorPositionsBlock   { vec4  donorSolutionPositions  []; };
layout (std430,  binding = 4) buffer donorNormalsBlock     { vec4  donorSolutionNormals    []; };
layout (std430,  binding = 5) buffer donorXAxisBlock       { vec4  donorSolutionXAxes      []; };
layout (std430,  binding = 6) buffer donorSiteIndexBlock   { uint  donorSolutionSiteIndices[]; };

layout (std430,  binding = 7) buffer totalRmsBlock         { float totalRmsOutput[];         };
