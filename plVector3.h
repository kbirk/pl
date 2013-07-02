#ifndef __PL_VECTOR3_H__
#define __PL_VECTOR3_H__

#include <iostream>
#include <cstdlib>
#include <cfloat>
#include <cstdio>
#include <cmath>

#include "pl.h"

class plVector3 
{
	public:

		PLfloat x, y, z;

		plVector3();
		plVector3( PLfloat xx, PLfloat yy, PLfloat zz );
		plVector3( PLchar *string );  
		plVector3( plString str );

		PLbool 	  operator == (const plVector3 &p) const;
		PLbool 	  operator != (const plVector3 &p) const;
		plVector3 operator +  (const plVector3 &p) const;
		plVector3 operator -  (const plVector3 &p) const;
		PLfloat   operator *  (const plVector3 &p) const;
		plVector3 operator ^  (const plVector3 &p) const;
        
		plVector3 operator-() const;		

		plVector3 normalize() const;
		PLfloat length() const;
		PLfloat squaredLength() const;
	  
		plVector3 perp1() const;
		plVector3 perp2() const;
  
};


// Scalar/Vector multiplication

plVector3 operator * ( PLfloat k, const plVector3 &p );

PLfloat distToEdge( const plVector3 &point, const plVector3 &head, const plVector3 &tail );

// I/O operators

std::ostream& operator << ( std::ostream& stream, const plVector3 &p );
std::istream& operator >> ( std::istream& stream, plVector3 &p );


#endif
