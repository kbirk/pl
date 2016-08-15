
// uniforms

uniform uint  uSeed;

// functions

uint generateSeed();
uint iterateSeed(inout uint seed);
float rand(inout uint seed);
float rand(inout uint seed, in float min, in float max);
