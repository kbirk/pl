#ifndef __PL_MATRIX44_H__
#define __PL_MATRIX44_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plTriangle.h"

class plMatrix44 
{
	public:
		
		// 4x4 matrix - column major
		//	0	4	8	12
		//	1	5	9	13
		//	2	6	10	14
		//	3	7	11	15		

		plMatrix44();                                   // identity 
		
		plMatrix44(PLfloat scale);                      // scale 		 
		plMatrix44(PLfloat x, PLfloat y, PLfloat z);    // translation
		plMatrix44(const plVector3 &translation);       // translation		 
		plMatrix44(PLfloat angle, const plVector3 &axis);                                               // rotation		
		plMatrix44(const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &t);     // affine transformation 		
		plMatrix44(PLfloat xMin, PLfloat xMax, PLfloat yMin, PLfloat yMax, PLfloat zMin, PLfloat zMax); // orthographic projection matrix		
		plMatrix44(PLfloat fov, PLfloat aspectRatio, PLfloat zMin, PLfloat zMax);                       // perspective projection matrix		
		plMatrix44(PLfloat c1r1, PLfloat c2r1, PLfloat c3r1, PLfloat c4r1, 
                   PLfloat c1r2, PLfloat c2r2, PLfloat c3r2, PLfloat c4r2, 
			       PLfloat c1r3, PLfloat c2r3, PLfloat c3r3, PLfloat c4r3, 
			       PLfloat c1r4, PLfloat c2r4, PLfloat c3r4, PLfloat c4r4);
			       
		plMatrix44(const plMatrix44 &m);            // copy constructor

        PLbool importFile( const plString &file );  // openCV xml input file

		void setRow(int row, PLfloat x, PLfloat y, PLfloat z, PLfloat w);
		void setColumn(int col, PLfloat x, PLfloat y, PLfloat z, PLfloat w);

		void setZero        ();
		void setIdentity    ();
		void setTranslation ( PLfloat x, PLfloat y, PLfloat z);
		void setTranslation ( const plVector3 &t);
		void setScale       ( PLfloat scale);
		void setScale       ( PLfloat xScale, PLfloat yScale, PLfloat zScale);
        void setScale       ( const plVector3 s);

        void setRotation ( PLfloat angle_radian, const plVector3 &axis);
        void setRotation ( const plVector4 &quat );
        void setRotationD( PLfloat angle_degree, const plVector3 &axis);
        void setRotation ( const plVector3 &from, const plVector3 &to);
        
        void setOrthographic( PLfloat xMin, PLfloat xMax, PLfloat yMin, PLfloat yMax, PLfloat zMin, PLfloat zMax);
        void setPerspective ( PLfloat fov, PLfloat aspect, PLfloat nearPlane, PLfloat farPlane);

		plMatrix44 inverse() const;

		plMatrix44 &operator= ( const plMatrix44 &m); 
		PLfloat&    operator[]( int index)			   { return _data[index]; }
		PLfloat     operator[]( int index) const	       { return _data[index]; }
		PLfloat&    operator()( int row, int col)	   { return _data[(col*4) + row]; }
		PLfloat     operator()( int row, int col) const { return _data[(col*4) + row]; }

		plMatrix44  operator- () const;        // negation
		plMatrix44  operator+ ( const plMatrix44 &m) const;
		plMatrix44  operator- ( const plMatrix44 &m) const;
		plMatrix44  operator* ( PLfloat s) const;
		plMatrix44 &operator*=( PLfloat s);
		plVector3   operator* ( const plVector3 &v) const;
        plVector4   operator* ( const plVector4 &v) const;
        plTriangle  operator* ( const plTriangle &tri) const;
		plMatrix44  operator* ( const plMatrix44 &m) const;
        plSeq<plTriangle> operator*(const plSeq<plTriangle> &tris) const;

	protected:

		PLfloat _data[16];

};

std::ostream& operator << ( std::ostream& stream, const plMatrix44 &m );

plVector4 operator * ( const plVector4 &v, const plMatrix44 &m );
plVector3 operator * ( const plVector3 &v, const plMatrix44 &m );

#endif
