

// uniforms

uniform uint  uRotationIndex;
uniform float uRotationAngles[PL_NUM_COMPARISION_DIRECTIONS];

// functions

void setRMS(in float rms, in uint graftIndex);
float getRMS(in uint rotationIndex, in uint gridPointIndex, in uint graftIndex);

float getRotationAngle();
float getRotationAngle(in uint rotationIndex);
