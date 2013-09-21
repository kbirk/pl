#include "plMath.h"

namespace plMath
{

    void _concavePolysToTrisHelper( plSeq<plTriangle> &triangles, PLfloat &minSurfaceArea, const plPolygon &polygon );
   

    plVector3 projectVectorOnPlane( const plVector3 &vector, const plVector3 &plane_normal)
    {
        PLfloat dist = vector * plane_normal;
        
        return vector - (dist * plane_normal);
    }


    plVector3 closestPointOnSegment(const plVector3 &point, const plVector3 &a, const plVector3 &b)
    {
        plVector3 ab = b - a;
        // Project c onto ab, computing parameterized position d(t) = a + t*(b * a)
        PLfloat t = ((point - a) * ab) / (ab * ab);
        // If outside segment, clamp t (and therefore d) to the closest endpoint
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        // Compute projected position from the clamped t
        return a + t * ab;
    }

    PLbool closestPointsBetweenSegments(const plVector3 &edge1Point1, const plVector3 &edge1Point2, const plVector3 &edge2Point1, const plVector3 &edge2Point2, plVector3& closestPointEdge1, plVector3& closestPointEdge2, PLfloat& distanceBetweenLines)
    {
        plVector3 edge1Direction = edge1Point2 - edge1Point1;
        plVector3 edge2Direction = edge2Point2 - edge2Point1;

        plVector3 symbolicIntersectionDirection = edge1Direction ^ edge2Direction;

        // if lines are parallel, then there is no clear closest point between the two
        if (symbolicIntersectionDirection == plVector3(0,0,0))
        {
            distanceBetweenLines = (closestPointOnSegment(edge2Point1,edge1Point1,edge1Point2) - edge2Point1).length();
            return false;
        }

        plMatrix44 linearEquationMatrix(
                    -edge1Direction.x                , -edge1Direction.y                ,   -edge1Direction.z                , 0.f,
                     edge2Direction.x                ,  edge2Direction.y                ,    edge2Direction.z                , 0.f,
                     symbolicIntersectionDirection.x ,  symbolicIntersectionDirection.y ,    symbolicIntersectionDirection.z , 0.f,
                     0.f, 0.f, 0.f, 1.f);
        plVector3 linearEquationLeftSide = edge1Point1-edge2Point1;

        plVector3 params = linearEquationMatrix.inverse() * linearEquationLeftSide;
        // params.x is the parameter along edge1
        // params.y is the parameter along edge2

        closestPointEdge1 = edge1Point1 + (params.x * edge1Direction);
        closestPointEdge2 = edge2Point1 + (params.y * edge2Direction);
        distanceBetweenLines = (closestPointEdge1 - closestPointEdge2).length();
        return true;

    }
    

    PLfloat clamp( PLfloat val, PLfloat min, PLfloat max)
    {
        if (val > max) return max;
        if (val < min) return min;
        return val;
    }


    void swap( PLfloat &a, PLfloat &b )
    {
        PLfloat temp = a;
        a = b; 
        b = temp;
    }


    void shuffle( plSeq<PLuint> &array )
    {   
        PLuint size = array.size();
        for (PLuint i = 0; i < size-1; i++) 
        {
            PLuint j = i + rand() / (RAND_MAX / (size - i) + 1);
            PLuint t = array[j];
            array[j] = array[i];
            array[i] = t;
        }    
    }


    PLfloat fsqrt(PLfloat x)
    {
        #define SQRT_MAGIC_F 0x5f3759df   
        const PLfloat xhalf = 0.5f*x;
     
        union // get bits for floating value
        {
            PLfloat x;
            int i;
        } u;
        u.x = x;
        u.i = SQRT_MAGIC_F - (u.i >> 1);        // gives initial guess y0
        return x*u.x*(1.5f - xhalf*u.x*u.x);    // Newton step, repeating increases accuracy 
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

    plIntersection rayIntersect( const plSeq<plTriangle> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay, PLbool backFaceCull )
    {
        PLfloat min = FLT_MAX;
        plIntersection closestIntersection( false );

        for ( PLuint i = 0; i < triangles.size(); i++)
        {  
            plIntersection intersection = triangles[i].rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );
            
            if (intersection.exists)
            {
                PLfloat tAbs = fabs(intersection.t);
                if ( tAbs < min) 
                {
                    min = tAbs;
                    closestIntersection = intersection;
                }
            }

        }
        
        return closestIntersection;
    }   



    void convexPolysToTris( plSeq<plTriangle> &tris, const plSeq<plPolygon> &polys )
    {
        tris.clear();
        for ( PLuint i = 0; i < polys.size(); i++ )
        {
            if ( polys[i].points.size() > 2 ) // general case
            {
                plVector3 normal(polys[i].normal);
                plVector3 point0(polys[i].points[0]);
                // create a fan of triangles from this point
                for ( PLuint currentVertex=1; currentVertex<polys[i].points.size()-1; currentVertex++ )
                {
                    plVector3 point1( polys[i].points[currentVertex] );
                    plVector3 point2( polys[i].points[currentVertex+1] );
                    plTriangle tri(normal,point0,point1,point2);
                    tris.add(tri);
                }
            }
            else // must be an invalid polygon
            {
                std::cerr << " plMath::convexPolysToTris() error: polygon with less than three vertices detected: " << polys[i].points.size() << std::endl;
            }
        }
    }


    void concavePolysToTris( plSeq<plTriangle> &tris, const plSeq<plPolygon> &polys )
    {
        tris.clear();
        for (PLuint i = 0; i < polys.size(); i++)
        {
            plSeq<plTriangle> polyConverted;
            PLfloat area = FLT_MAX;
            _concavePolysToTrisHelper( polyConverted, area, polys[i] );
            for (PLuint j = 0; j < polyConverted.size(); j++)
            {
                tris.add(polyConverted[j]);
            }
        }
    }

    // take as input a polygon
    // iterate through each possible triangle involving the vertices at indices 0 and 1 (so 0-1-2, 0-1-3, and so on, basically we have 0-1-X)
    // there will be two polygons remaining:
    // the one from index 1 to X
    // the one from index X to 0
    // recursively call this function for each of these smaller polygons
    // store only the sequence of triangles that results in the smallest surface area
    // also store the surface area of the triangulation
    // INITIAL VALUE FOR triangles SHOULD BE AN EMPTY PLSEQ!
    // INITIAL VALUE FOR minSurfaceArea SHOULD BE THE MAXIMUM VALUE FOR A FLOATING POINT
    void _concavePolysToTrisHelper( plSeq<plTriangle> &triangles, PLfloat &minSurfaceArea, const plPolygon &polygon )
    {
        const float epsilon(0.0001f);

        if (polygon.points.size() <= 2) { // base case
            minSurfaceArea = 0.f;
            return;
        }
        // three points lie on a line
        if (polygon.points.size() == 3 && (polygon.points[2]^polygon.points[0]).length() <= epsilon && (polygon.points[2]-polygon.points[1]).length() <= epsilon)
        {
            minSurfaceArea = FLT_MAX/2.f;
            return;
        } 

        // try all permutations of triangles involving the edge between points[0] and points[1]
        for (PLuint i = 2; i < polygon.points.size(); i++)
        {
            plTriangle bisectingTriangle( polygon.points[0], polygon.points[1], polygon.points[i] );
            PLfloat    bisectingTriangleArea = bisectingTriangle.getArea(); 

            if (bisectingTriangleArea <= epsilon)
                continue; // bad triangle

            // first polygon
            plPolygon poly0;
            for (PLuint j = 1; j <= i; j++)
            {
                poly0.points.add( polygon.points[j] );
            }
            plSeq<plTriangle> poly0Tris;
            PLfloat           poly0Area = FLT_MAX;
            _concavePolysToTrisHelper( poly0Tris, poly0Area, poly0 );

            // second polygon
            plPolygon poly1;
            poly1.points.add( polygon.points[0] );
            for (PLuint j = i; j < polygon.points.size(); j++)
            {
                poly1.points.add( polygon.points[j] );
            }
            plSeq<plTriangle> poly1Tris;
            PLfloat           poly1Area = FLT_MAX;
            _concavePolysToTrisHelper( poly1Tris, poly1Area, poly1 );

            // calculate current surface
            PLfloat currentSurfaceArea( bisectingTriangleArea + poly0Area + poly1Area );

            // update the output if necessary
            if ( currentSurfaceArea < minSurfaceArea )
            {
                triangles.clear();
                triangles.add( bisectingTriangle );
                for (PLuint j = 0; j < poly0Tris.size(); j++)
                {
                    triangles.add( poly0Tris[j] );
                }
                for (PLuint j = 0; j < poly1Tris.size(); j++)
                {
                    triangles.add( poly1Tris[j] );
                }
                minSurfaceArea = currentSurfaceArea;
            }
        }

    }

}




