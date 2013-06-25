#include "plMath.h"

plVector3 plProjectVectorOnPlane( const plVector3 &vector, const plVector3 &plane_normal)
{
    PLfloat dist = vector * plane_normal;
    
    return vector - (dist*plane_normal);
}

plVector3 plClosestPointOnLineSegment(const plVector3 &point, const plVector3 &a, const plVector3 &b)
{
    plVector3 ab = b - a;
    // Project c onto ab, computing parameterized position d(t) = a + t*(b  a)
    PLfloat t = ((point - a) * ab) / (ab * ab);
    // If outside segment, clamp t (and therefore d) to the closest endpoint
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    // Compute projected position from the clamped t
    return a + t * ab;
}

/*
plVector3 plClosestPointOnPlane(const plVector3 &q, const plVector3 &p, const plVector3 &n)
{
    float t = 1/(n * n) * (n*(q - p));
    return q - t * n;
}
*/
