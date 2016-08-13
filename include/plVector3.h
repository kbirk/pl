#ifndef PL_VECTOR3_H
#define PL_VECTOR3_H

#include "plCommon.h"
#include "plString.h"

class plVector4;    // forward delcaration to allow plVector4 -> plVector3 casting

class plVector3
{
	public:

		PLfloat x, y, z;

		plVector3();
		plVector3( PLfloat xx, PLfloat yy, PLfloat zz );
		plVector3( const plVector4& v );
		plVector3( PLchar *string );
		plVector3( const plString &str );

        PLbool 	   operator == (const plVector3 &p) const;
        PLbool 	   operator != (const plVector3 &p) const;
        plVector3  operator +  (const plVector3 &p) const;
        plVector3  operator -  (const plVector3 &p) const;
        PLfloat    operator *  (const plVector3 &p) const;
        plVector3  operator ^  (const plVector3 &p) const;

        plVector3& operator =  (const plVector3 &p);

		plVector3 operator-() const;

		plVector3 normalize() const;
		PLfloat length() const;
		PLfloat squaredLength() const;
	    PLfloat signedAngle( const plVector3 v, const plVector3& planeNormal ) const;

};

// scalar multiplication
plVector3 operator * ( PLfloat k, const plVector3 &p );

// I/O operators
std::ostream& operator << ( std::ostream& stream, const plVector3 &p );
std::istream& operator >> ( std::istream& stream, plVector3 &p );


#endif
