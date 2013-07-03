#ifndef __PL_MATRIX44_H__
#define __PL_MATRIX44_H__

#include "pl.h"
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

		////////////////////////////////////////////////////////
		// constructors
		////////////////////////////////////////////////////////

		// identity 
		plMatrix44() 
		{
			// default to identity  
			setIdentity();
		}
		// scale 
		plMatrix44(PLfloat scale)
		{
			setScale(scale);
		}
		// translation 
		plMatrix44(PLfloat x, PLfloat y, PLfloat z)
		{
			setTranslation(x, y, z);
		}
		plMatrix44(const plVector3 &translation)
		{
			setTranslation(translation);
		}
		// rotation 
		plMatrix44(PLfloat angle, const plVector3 &axis)
		{
			setRotation(angle, axis);
		}
		// affine transformation 
		plMatrix44(const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &t)
		{
			_data[0] = x.x;	_data[4] = y.x;	_data[8]  = z.x;	_data[12] = t.x;
			_data[1] = x.y;	_data[5] = y.y;	_data[9]  = z.y;	_data[13] = t.y;
			_data[2] = x.z;	_data[6] = y.z;	_data[10] = z.z;	_data[14] = t.z;
			_data[3] = 0.0f;_data[7] = 0.0f;_data[11] = 0.0f;	_data[15] = 1.0f;
		}
		// orthographic projection matrix
		plMatrix44(PLfloat xMin, PLfloat xMax, PLfloat yMin, PLfloat yMax, PLfloat zMin, PLfloat zMax)
		{
			setOrthographic(xMin, xMax, yMin, yMax, zMin, zMax);
		}
		// perspective projection matrix
		plMatrix44(PLfloat fov, PLfloat aspectRatio, PLfloat zMin, PLfloat zMax)
		{
			setPerspective(fov, aspectRatio, zMin, zMax);
		}

		plMatrix44(PLfloat c1r1, PLfloat c2r1, PLfloat c3r1, PLfloat c4r1, 
                   PLfloat c1r2, PLfloat c2r2, PLfloat c3r2, PLfloat c4r2, 
			       PLfloat c1r3, PLfloat c2r3, PLfloat c3r3, PLfloat c4r3, 
			       PLfloat c1r4, PLfloat c2r4, PLfloat c3r4, PLfloat c4r4) 
		{
			_data[0] = c1r1;	_data[4] = c2r1;	_data[8]  = c3r1;	_data[12] = c4r1;
			_data[1] = c1r2;	_data[5] = c2r2;	_data[9]  = c3r2;	_data[13] = c4r2;
			_data[2] = c1r3;	_data[6] = c2r3;	_data[10] = c3r3;	_data[14] = c4r3;
			_data[3] = c1r4;	_data[7] = c2r4;	_data[11] = c3r4;	_data[15] = c4r4;
		}
		
		plMatrix44( double data[16] )
		{		    
		    _data[0] = data[0];	_data[4] = data[4];	_data[8]  = data[8];	_data[12] = data[12];
			_data[1] = data[1];	_data[5] = data[5];	_data[9]  = data[9];	_data[13] = data[13];
			_data[2] = data[2];	_data[6] = data[6];	_data[10] = data[10];	_data[14] = data[14];
			_data[3] = data[3];	_data[7] = data[7];	_data[11] = data[11];	_data[15] = data[15];
		}   
		
		// copy constructor
		plMatrix44(const plMatrix44 &m) 
		{			
			memcpy(_data, m._data, sizeof(PLfloat)*16);
		}
        
		////////////////////////////////////////////////////////
		// getters
		////////////////////////////////////////////////////////
		inline const PLfloat(&data() const)[16] { return _data; }      

		////////////////////////////////////////////////////////
		// setters
		////////////////////////////////////////////////////////
		inline void setRow(int row, PLfloat x, PLfloat y, PLfloat z, PLfloat w)  
		{
			// copy plVector3 into specified row, rows indexed 0-3
			_data[row]	   = x;
			_data[row+4]  = y;
			_data[row+8]  = z;
			_data[row+12] = w;
		}

		inline void setColumn(int col, PLfloat x, PLfloat y, PLfloat z, PLfloat w)
		{
			// copy plVector3 into specified column, columns indexed 0-3
			_data[(col*4)]   = x;
			_data[(col*4)+1] = y;
			_data[(col*4)+2] = z;
			_data[(col*4)+3] = w;
		}


		inline void setZero()
		{ 
			memset(_data, 0, sizeof(PLfloat)*16);
		}

		inline void setIdentity() 
		{
			// set to identity plMatrix44
			setZero();
			_data[0]  = 1.0f;
			_data[5]  = 1.0f;
			_data[10] = 1.0f;
			_data[15] = 1.0f;
		}

		inline void setTranslation(PLfloat x, PLfloat y, PLfloat z)
		{
			setIdentity();
			_data[12] = x;
			_data[13] = y;
			_data[14] = z;
		}
		inline void setTranslation(const plVector3 &t)
		{
			setTranslation(t.x, t.y, t.z);
		}

		inline void setScale(PLfloat scale)
		{
			// create scaling plMatrix44
			setIdentity();
			_data[0]  = scale;
			_data[5]  = scale;
			_data[10] = scale;
		}

		inline void setScale(PLfloat xScale, PLfloat yScale, PLfloat zScale)
		{
			// create scaling plMatrix44
			setIdentity();
			_data[0]  = xScale;
			_data[5]  = yScale;
			_data[10] = zScale;
		}

        inline void setScale(const plVector3 s)
		{
			// create scaling plMatrix44
			setScale(s.x, s.y, s.z);
		}

		//void setRotation(PLfloat angle, PLfloat x, PLfloat y, PLfloat z);
        void setRotation(PLfloat angle_radian, const plVector3 &axis);
        void setRotation( const plVector4 &quat );
        void setRotationD(PLfloat angle_degree, const plVector3 &axis);
        void setRotation(const plVector3 &from, const plVector3 &to);
        
        void setOrthographic(PLfloat xMin, PLfloat xMax, PLfloat yMin, PLfloat yMax, PLfloat zMin, PLfloat zMax);
        void setPerspective(PLfloat fov, PLfloat aspect, PLfloat nearPlane, PLfloat farPlane);

		///////////////////////////////////////////////////////
		// inverse
		///////////////////////////////////////////////////////
		plMatrix44 inverse() const;

		////////////////////////////////////////////////////////
		// operator overloading
		////////////////////////////////////////////////////////

		// assignment
		inline plMatrix44 operator=(const plMatrix44 &m) 
		{ 
			// copy data
			memcpy(_data, m._data, sizeof(PLfloat)*16);
			return *this;
		} 

		// set/get operators 
		inline PLfloat& operator[](int index)			   { return _data[index]; }
		inline PLfloat  operator[](int index) const	       { return _data[index]; }
		inline PLfloat& operator()(int row, int col)	   { return _data[(col*4) + row]; }
		inline PLfloat  operator()(int row, int col) const { return _data[(col*4) + row]; }

		////////////////////////////////////////////////////////
		// unary operators 
		////////////////////////////////////////////////////////

		// negation
		inline plMatrix44 operator-() const 
		{
			return plMatrix44(-_data[0], -_data[1], -_data[2], -_data[3],
							-_data[4], -_data[5], -_data[6], -_data[7],
							-_data[8], -_data[9], -_data[10], -_data[11],
							-_data[12], -_data[13], -_data[14], -_data[15]);
		} 

		////////////////////////////////////////////////////////
		// binary operators
		////////////////////////////////////////////////////////

		// addition 
		inline plMatrix44 operator+(const plMatrix44 &m) const 
		{
			return plMatrix44(_data[0] + m._data[0], _data[1] + m._data[1], _data[2] + m._data[2], _data[3] + m._data[3],
							  _data[4] + m._data[4], _data[5] + m._data[5], _data[6] + m._data[6], _data[7] + m._data[7],
						      _data[8] + m._data[8], _data[9] + m._data[9], _data[10] + m._data[10], _data[11] + m._data[11],
							  _data[12] + m._data[12], _data[13] + m._data[13], _data[14] + m._data[14], _data[15] + m._data[15]);
		}
		// subtraction
		inline plMatrix44 operator-(const plMatrix44 &m) const 
		{
			return plMatrix44(_data[0] - m._data[0],   _data[1] - m._data[1],   _data[2] - m._data[2],   _data[3] - m._data[3],
							  _data[4] - m._data[4],   _data[5] - m._data[5],   _data[6] - m._data[6],   _data[7] - m._data[7],
							  _data[8] - m._data[8],   _data[9] - m._data[9],   _data[10] - m._data[10], _data[11] - m._data[11],
							  _data[12] - m._data[12], _data[13] - m._data[13], _data[14] - m._data[14], _data[15] - m._data[15]);
		}
		// scalar multiplication
		inline plMatrix44 operator*(PLfloat s) const 
		{
			return plMatrix44(_data[0] * s, _data[1] * s, _data[2] * s,  _data[3] * s,
							  _data[4] * s, _data[5] * s, _data[6] * s,  _data[7] * s,
							  _data[8] * s, _data[9] * s, _data[10] * s, _data[11] * s,
							  _data[12],	   _data[13],    _data[14],     _data[15]);

		}
		inline plMatrix44 &operator*=(PLfloat s)  
		{
			_data[0] *= s;	_data[4] *= s;	_data[8] *= s;	
			_data[1] *= s;	_data[5] *= s;	_data[9] *= s;		
			_data[2] *= s;	_data[6] *= s;	_data[10] *= s; 	
			_data[3] *= s;	_data[7] *= s;	_data[11] *= s; 	
			return *this;
		}

		// plVector3 multiplication
		inline plVector3 operator*(const plVector3 &v) const 
		{
			return plVector3((_data[0] * v.x) + (_data[4] * v.y) + (_data[8]  * v.z) + _data[12],
						     (_data[1] * v.x) + (_data[5] * v.y) + (_data[9]  * v.z) + _data[13],
						     (_data[2] * v.x) + (_data[6] * v.y) + (_data[10] * v.z) + _data[14]);
		}

        inline plVector4 operator*(const plVector4 &v) const 
		{
			return plVector4((_data[0] * v.x) + (_data[4] * v.y) + (_data[8]  * v.z) + (_data[12] * v.w),
						     (_data[1] * v.x) + (_data[5] * v.y) + (_data[9]  * v.z) + (_data[13] * v.w),
						     (_data[2] * v.x) + (_data[6] * v.y) + (_data[10] * v.z) + (_data[14] * v.w),
						     (_data[3] * v.x) + (_data[7] * v.y) + (_data[11] * v.z) + (_data[15] * v.w));
		}

        // triangle multiplication
        inline plTriangle operator*(const plTriangle &tri) const
        {
            plVector3 point0  = *this * tri.point0();
            plVector3 point1  = *this * tri.point1();
            plVector3 point2  = *this * tri.point2();

            // normal is special, we must have 0 in the final entry (default is 1 for a plVector3)
            plVector4 normal4 = *this * plVector4( tri.normal().x,  tri.normal().y,  tri.normal().z, 0.);
            plVector3 normal3 =         plVector3(      normal4.x,       normal4.y,       normal4.z );

            return plTriangle(normal3,point0,point1,point2);
        }

		////////////////////////////////////////////////////////
		// plMatrix44 multiplication
		////////////////////////////////////////////////////////
		plMatrix44 operator*(const plMatrix44 &m) const 
        {
            plMatrix44 result;
            for (int i = 0; i < 4; i++) 
            {
                result._data[i]    = _data[i] * m._data[0] + _data[i+4] * m._data[1] + _data[i+8] * m._data[2] + _data[i+12] * m._data[3];
                result._data[i+4]  = _data[i] * m._data[4] + _data[i+4] * m._data[5] + _data[i+8] * m._data[6]+ _data[i+12] * m._data[7];
                result._data[i+8]  = _data[i] * m._data[8] + _data[i+4] * m._data[9] + _data[i+8] * m._data[10] + _data[i+12] * m._data[11];
                result._data[i+12] = _data[i] * m._data[12] + _data[i+4] * m._data[13] + _data[i+8] * m._data[14] + _data[i+12] * m._data[15];
            }
            return result;
        }

	protected:

		PLfloat _data[16];

};

plVector4 operator * ( const plVector4 &v, const plMatrix44 &m );


#endif
