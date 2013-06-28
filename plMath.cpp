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
int plPlaneIntersection( const plvector3 &p1_normal, const plvector3 &p2_normal, Plane p1, Plane p2, Point &p, Vector &d)
{
    const PLfloat EPSILON = 0.000001f;
    // Compute direction of intersection line
    plVector3 d = p1_normal ^  p2_normal;
    // If d is zero, the planes are parallel (and separated)
    // or coincident, so they’re not considered intersecting
    if (d * d < EPSILON) 
        return 0;
        
    PLfloat d11 = p1_normal * p1_normal;
    PLfloat d12 = p1_normal * p2_normal;
    PLfloat d22 = p2_normal * p2_normal;
    PLfloat denom = d11*d22 - d12*d12;
    PLfloat k1 = (p1.d*d22 - p2.d*d12) / denom;
    PLfloat k2 = (p2.d*d11 - p1.d*d12) / denom;
    p = k1*p1.n + k2*p2.n;
    return 1;
}

plVector3 plClosestPointOnPlane(const plVector3 &q, const plVector3 &p, const plVector3 &n)
{
    float t = 1/(n * n) * (n*(q - p));
    return q - t * n;
}
*/
