#include "plMath.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plMatrix44.h"
#include "plTriangle.h"
#include "plPolygon.h"

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


    plVector3 closestPointOnLine( const plVector3 &point, const plVector3 &lineOrigin, const plVector3& lineDirection )
    {
        // Project c onto ab, computing parameterized position d(t) = a + t*(b * a)
        PLfloat t = ((point - lineOrigin) * lineDirection) / (lineDirection * lineDirection);
        // If outside segment, clamp t (and therefore d) to the closest endpoint
        // Compute projected position from the clamped t
        return lineOrigin + t * lineDirection;
    }
   

    PLbool closestPointsBetweenSegments( const plVector3 &edge1Point1, 
                                         const plVector3 &edge1Point2, 
                                         const plVector3 &edge2Point1, 
                                         const plVector3 &edge2Point2, 
                                         plVector3& closestPointEdge1,
                                         plVector3& closestPointEdge2, 
                                         PLfloat& distanceBetweenLines)
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


    PLfloat fsqrt( PLfloat x )
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


    plVector3 closestPointOnPlane(const plVector3 &lineDirection, const plVector3 &linePoint, const plVector3 &planeNormal, const plVector3 &planePoint)
    {
        PLfloat t = ((planePoint - linePoint)*planeNormal)/(lineDirection*planeNormal);
        return (t * lineDirection) + linePoint;
    }

    
    PLbool solveMatrix22Equation(PLfloat a11, PLfloat a12, PLfloat a21, PLfloat a22, PLfloat b1, PLfloat b2, PLfloat &x, PLfloat &y)
    {
        // solve for x and y in this thing:
        //    [ a11 a12 ] [ x ]   [ b1 ]
        //    [ a21 a22 ] [ y ] = [ b2 ]
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
    } 


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



    plIntersection rayIntersect( const std::vector<plTriangle>& triangles, 
                                 const plVector3 &rayOrigin, 
                                 const plVector3 &rayDirection,
                                 PLbool smoothNormal, 
                                 PLbool ignoreBehindRay, 
                                 PLbool backFaceCull )
    {
        PLfloat min = FLT_MAX;
        plIntersection closestIntersection( false );

        for ( const plTriangle& triangle : triangles )
        {  
            plIntersection intersection = triangle.rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );           
            if ( intersection.exists )
            {
                PLfloat tAbs = fabs(intersection.t);
                if ( tAbs < min) 
                {
                    min = tAbs;
                    closestIntersection = intersection;
                }
            }
        }        

        if ( smoothNormal )
            closestIntersection.normal = plMath::getAverageNormal( triangles, PL_NORMAL_SMOOTHING_RADIUS, closestIntersection.point, closestIntersection.normal );

        return closestIntersection;
    } 


    plIntersection rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, const plVector3& planePoint, const plVector3& planeNormal )
    {
        if ( planeNormal * rayDirection == 0 )
            return plIntersection( false );
            
        // Compute the t value for the directed line ab intersecting the plane
        PLfloat t = ( (planePoint * planeNormal) - (planeNormal * rayOrigin) ) / ( planeNormal * rayDirection );
        plVector3 intPoint = rayOrigin + t * rayDirection;
        return plIntersection( intPoint, planeNormal, t );
    }


    plVector3 getAverageNormal( const std::vector<plTriangle>& triangles, PLfloat radius, const plVector3 &origin, const plVector3 &normal )
    {
        plVector3 avgNormal( 0, 0, 0 );
        PLint count = 0;
        float radiusSquared = radius * radius;
    
        // Find polygons on top of graft
        for ( const plTriangle& triangle : triangles )
        {
            if ( triangle.normal() * normal > 0.001)
            {        
                PLfloat dist1 = ( triangle.point0() - origin ).squaredLength();
                PLfloat dist2 = ( triangle.point1() - origin ).squaredLength();
                PLfloat dist3 = ( triangle.point2() - origin ).squaredLength();
           
                // if any point of triangle is withing radial sphere, accept
                float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

                if ( minDist <= radiusSquared )
                {        
                    avgNormal = avgNormal + triangle.normal();
                    count++;
                }
            }
        } 

        if ( count == 0 )
        {
            // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
            //std::cout << "plMath::getAverageNormal() warning: No normal found" << std::endl;
            return normal;
        }    

        return ( 1.0f/(PLfloat)(count) * avgNormal ).normalize();
    }


    plIntersection getClosestPointToRay( const std::vector<plTriangle>& triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection )
    {
        plIntersection intersection( false );
    
        PLfloat lowestDist = FLT_MAX;
               
        for ( const plTriangle& triangle : triangles )
        {
            // find closest point on ray from triangle centre
            plVector3 closestPointOnLine = plMath::closestPointOnLine( triangle.centroid(), rayOrigin, rayDirection );
                
            // find point on sphere closest to point
            plVector3 closestPointOnSphere = triangle.centroid() + ( closestPointOnLine - triangle.centroid() ).normalize() * triangle.radius();
            
            // find distance
            PLfloat dist = ( closestPointOnSphere - closestPointOnLine ).squaredLength(); 
                
            if ( dist < lowestDist )
            {
                // if less than current, find actual closest point on triangle
                
                plVector3 closestPointOnTri = triangle.closestPointTo( closestPointOnLine );
            
                lowestDist = ( closestPointOnTri - closestPointOnLine ).squaredLength(); ;
                intersection.exists = true;
                intersection.point  = closestPointOnTri;
                intersection.normal = triangle.normal();
            }
        }
        
        return intersection;
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




