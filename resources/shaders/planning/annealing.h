

// functions

float energy (in state s);
void calculateCapAreaCoverage(in vec4 position, in vec4 normal, in float radius);
float uncoveredArea(in state s);
float graftOverlap(in state s);
void setCapOverlap(in uint index, in float value);
float getCapOverlap(in uint index);
float acceptanceProbability(in float energy, in float newEnergy);
