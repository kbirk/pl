#include "plMath.h"

namespace plMath
{

    void _concavePolysToTrisHelper( std::vector<plTriangle> &triangles, PLfloat &minSurfaceArea, const plPolygon &polygon );
   

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
        PLfloat t1, t2;
        if (!intersectTwoLines(edge1Point1, edge2Point1, edge1Direction, edge2Direction, t1,t2))
            return false;

        if (t1 < 0.f)
            t1 = 0.f;
        else if (t1 > 1.f)
            t1 = 1.f;
        if (t2 < 0.f)
            t2 = 0.f;
        else if (t2 > 1.f)
            t2 = 1.f;

        closestPointEdge1 = edge1Point1 + (t1 * edge1Direction);
        closestPointEdge2 = edge2Point1 + (t2 * edge2Direction);
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


    void shuffle( std::vector<PLuint> &array )
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

    plVector3 closestPointOnPlane(const plVector3 &lineDirection, const plVector3 &linePoint, const plVector3 &planeNormal, const plVector3 &planePoint)
    {
        PLfloat t = ((planePoint - linePoint)*planeNormal)/(lineDirection*planeNormal);
        return (t * lineDirection) + linePoint;
    }

    // solve for x and y in this thing:
    //    [ a11 a12 ] [ x ]   [ b1 ]
    //    [ a21 a22 ] [ y ] = [ b2 ]
    PLbool solveMatrix22Equation(PLfloat a11, PLfloat a12, PLfloat a21, PLfloat a22, PLfloat b1, PLfloat b2, PLfloat &x, PLfloat &y)
    {
        // basically, make the inverse, store in matrix d:
        PLfloat determinant = a11 * a22 - a21 * a12;

        if (fabs(determinant) <= PL_EPSILON) // singularity, no solution exists
            return false;

        PLfloat d11 (  a22 / determinant );
        PLfloat d12 ( -a12 / determinant );
        PLfloat d21 ( -a21 / determinant );
        PLfloat d22 (  a11 / determinant );

        // now if both sides are left-multiplied by d, we have:
        // [ x ]   [ d11 d12 ] [ b1 ]
        // [ y ] = [ d21 d22 ] [ b2 ]
        // which is our solution
        x = d11 * b1 + d12 * b2;
        y = d21 * b1 + d22 * b2;

        return true;
    } // end plSolveMatrix22Equation

    // method obtained from http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    PLbool intersectTwoLines(const plVector3 &edge1Point, const plVector3 &edge2Point, const plVector3 &edge1Direction, const plVector3 &edge2Direction, PLfloat &edge1Param, PLfloat &edge2Param)
    {
        // method obtained from http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
        edge1Param = 0.f;
        edge2Param = 0.f;
        plVector3 crossProductRightSideEdge1 ((edge2Point-edge1Point)^edge2Direction);
        plVector3 crossProductRightSideEdge2 ((edge2Point-edge1Point)^edge1Direction);
        plVector3 crossProductLeftSide  (edge1Direction^edge2Direction);
        PLfloat   numCalculations(0.f);
        if (fabs(crossProductLeftSide.x) > PL_EPSILON)
        {
            edge1Param += crossProductRightSideEdge1.x/crossProductLeftSide.x;
            edge2Param += crossProductRightSideEdge2.x/crossProductLeftSide.x;
            numCalculations+=1.f;
        }
        if (fabs(crossProductLeftSide.y) > PL_EPSILON)
        {
            edge1Param += crossProductRightSideEdge1.y/crossProductLeftSide.y;
            edge2Param += crossProductRightSideEdge2.y/crossProductLeftSide.y;
            numCalculations+=1.f;
        }
        if (fabs(crossProductLeftSide.z) > PL_EPSILON)
        {
            edge1Param += crossProductRightSideEdge1.z/crossProductLeftSide.z;
            edge2Param += crossProductRightSideEdge2.z/crossProductLeftSide.z;
            numCalculations+=1.f;
        }
        if (numCalculations == 0.f) // lines are parallel and do not intersect
            return false;

        edge1Param /= numCalculations;
        edge2Param /= numCalculations;

        return true;
    }

    // another version of the above function that may be worth trying, may be more accurate and/or faster
    /*PLbool intersectTwoLines(const plVector3 &edge1Point, const plVector3 &edge2Point, const plVector3 &edge1Direction, const plVector3 &edge2Direction, PLfloat &edge1Param, PLfloat &edge2Param)
    {
        // method obtained from http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
        edge1Param = 0.f;
        edge2Param = 0.f;
        plVector3 crossProductRightSideEdge1 ((edge2Point-edge1Point)^edge2Direction);
        plVector3 crossProductRightSideEdge2 ((edge2Point-edge1Point)^edge1Direction);
        plVector3 crossProductLeftSide  (edge1Direction^edge2Direction);
        PLfloat   maximumSolution(0.f);
        if (fabs(crossProductLeftSide.x) > PL_EPSILON && fabs(crossProductLeftSide.x) > maximumSolution)
        {
            maximumSolution = fabs(crossProductLeftSide.x);
            edge1Param = crossProductRightSideEdge1.x/crossProductLeftSide.x;
            edge2Param = crossProductRightSideEdge2.x/crossProductLeftSide.x;
        }
        if (fabs(crossProductLeftSide.y) > PL_EPSILON && fabs(crossProductLeftSide.y) > maximumSolution)
        {
            maximumSolution = fabs(crossProductLeftSide.y);
            edge1Param = crossProductRightSideEdge1.y/crossProductLeftSide.y;
            edge2Param = crossProductRightSideEdge2.y/crossProductLeftSide.y;
        }
        if (fabs(crossProductLeftSide.z) > PL_EPSILON && fabs(crossProductLeftSide.z) > maximumSolution)
        {
            maximumSolution = fabs(crossProductLeftSide.z);
            edge1Param = crossProductRightSideEdge1.z/crossProductLeftSide.z;
            edge2Param = crossProductRightSideEdge2.z/crossProductLeftSide.z;
        }
        if (maximumSolution <= PL_EPSILON) // lines are parallel and do not intersect
            return false;

        return true;
    }*/


    plIntersection rayIntersect( const std::vector<plTriangle> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay, PLbool backFaceCull )
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



    void convexPolysToTris( std::vector<plTriangle> &tris, const std::vector<plPolygon> &polys )
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
                    tris.push_back(tri);
                }
            }
            else // must be an invalid polygon
            {
                std::cerr << " plMath::convexPolysToTris() error: polygon with less than three vertices detected: " << polys[i].points.size() << std::endl;
            }
        }
    }


    void concavePolysToTris( std::vector<plTriangle> &tris, const std::vector<plPolygon> &polys )
    {
        tris.clear();
        for (PLuint i = 0; i < polys.size(); i++)
        {
            std::vector<plTriangle> polyConverted;
            PLfloat area = FLT_MAX;
            _concavePolysToTrisHelper( polyConverted, area, polys[i] );
            for (PLuint j = 0; j < polyConverted.size(); j++)
            {
                tris.push_back( polyConverted[j] );
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
    // INITIAL VALUE FOR triangles SHOULD BE AN EMPTY std::vector!
    // INITIAL VALUE FOR minSurfaceArea SHOULD BE THE MAXIMUM VALUE FOR A FLOATING POINT
    void _concavePolysToTrisHelper( std::vector<plTriangle> &triangles, PLfloat &minSurfaceArea, const plPolygon &polygon )
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
                poly0.points.push_back( polygon.points[j] );
            }
            std::vector<plTriangle> poly0Tris;
            PLfloat           poly0Area = FLT_MAX;
            _concavePolysToTrisHelper( poly0Tris, poly0Area, poly0 );

            // second polygon
            plPolygon poly1;
            poly1.points.push_back( polygon.points[0] );
            for (PLuint j = i; j < polygon.points.size(); j++)
            {
                poly1.points.push_back( polygon.points[j] );
            }
            std::vector<plTriangle> poly1Tris;
            PLfloat           poly1Area = FLT_MAX;
            _concavePolysToTrisHelper( poly1Tris, poly1Area, poly1 );

            // calculate current surface
            PLfloat currentSurfaceArea( bisectingTriangleArea + poly0Area + poly1Area );

            // update the output if necessary
            if ( currentSurfaceArea < minSurfaceArea )
            {
                triangles.clear();
                triangles.push_back( bisectingTriangle );
                for (PLuint j = 0; j < poly0Tris.size(); j++)
                {
                    triangles.push_back( poly0Tris[j] );
                }
                for (PLuint j = 0; j < poly1Tris.size(); j++)
                {
                    triangles.push_back( poly1Tris[j] );
                }
                minSurfaceArea = currentSurfaceArea;
            }
        }

    }

}




