#ifndef __PL_MATRIX_STACK__
#define __PL_MATRIX_STACK__

#include <stack>
#include "pl.h"
#include "plMatrix44.h"
#include "plVector3.h"

class plMatrixStack
{

	public:
		
		plMatrixStack(void)
		{
			// stack always starts with identity matrix
			_stack.push(plMatrix44());
		}

		////////////////////////////////////////////////////////
		// load matrix onto stack
		////////////////////////////////////////////////////////
		inline void loadIdentity(void)					{ _stack.top().setIdentity();	}		
		inline void loadMatrix(const plMatrix44 &m)	    { _stack.top() = m;			    }    

		////////////////////////////////////////////////////////
		// multiply current matrix
		////////////////////////////////////////////////////////
		inline void multMatrix(const plMatrix44 &m)     { _stack.top() = _stack.top() * m; }         

		// set /  get
		inline float& operator()(int row, int col)		 { return _stack.top()(row, col); }
		inline float  operator()(int row, int col) const { return _stack.top()(row, col); }

		////////////////////////////////////////////////////////
		// push / pop matrix stack
		////////////////////////////////////////////////////////
		inline void push(void)				    { _stack.push(_stack.top()); }	
		inline void push(const plMatrix44 &m)	{ _stack.push(m);			 }	

		inline void pop(void)
		{
			if (_stack.size() > 1)
			{
				_stack.pop();
			}
			else 
			{
				_stack.top().setIdentity(); // if stack has only 1 matrix, set it to identity
			}			
		}
		
		/*
		////////////////////////////////////////////////////////
		// scale / rotate / translate current matrix (float)
		////////////////////////////////////////////////////////
		void scale(GLfloat x, GLfloat y, GLfloat z)
		{
			Matrix44 scale;
			scale.setScale(x, y, z);
			_stack.top() = _stack.top() * scale;
		}			
		void translate(GLfloat x, GLfloat y, GLfloat z)
		{
			Matrix44 trans;
			trans.setTranslation(x, y, z);
			_stack.top() = _stack.top() * trans;		
		}			
		void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
		{
			Matrix44 rot;
			rotMat.setRotation(angle, plVector3(x, y, z) );
			_stack.top() = _stack.top() * rot;	
		}
        */
        
		////////////////////////////////////////////////////////
		// scale / rotate / translate current matrix (Vector)
		////////////////////////////////////////////////////////
		void scale(const plVector3 &v) 
		{
			plMatrix44 scale;
			scale.setScale(v);
			_stack.top() = _stack.top() * scale;
		}				
		void translate(const plVector3 &v) 
		{
			plMatrix44 trans;
			trans.setTranslation(v);
			_stack.top() = _stack.top() * trans;		
		}				
		void rotate(float angle, const plVector3 &v)
		{
			plMatrix44 rot;
			rot.setRotationD(angle, v);
			_stack.top() = _stack.top() * rot;	
		}				

		////////////////////////////////////////////////////////
		// getters
		////////////////////////////////////////////////////////
		inline const plMatrix44& getMatrix(void)	    { return _stack.top(); }
        inline const plMatrix44& top(void)				{ return _stack.top(); }

	private:

		std::stack<plMatrix44> _stack;

};

//////////////////////////////////////////////////

void plSetMatrix(const plMatrix44 &matrix)
{
	_plModelMatrixStack->loadMatrix( matrix );
}

void plMultMatrix(const plMatrix44 &matrix)
{
	_plModelMatrixStack->multMatrix( matrix );
}

void plPushMatrix()
{
	_plModelMatrixStack->push();
}

void plPopMatrix()
{
	_plModelMatrixStack->pop();
}

void plRotate(PLfloat angle_degree, const plVector3 &axis)
{
    _plModelMatrixStack->rotate(angle_degree, axis);
}

void plTranslate( const plVector3 &translation)
{
    _plModelMatrixStack->translate(translation);
}

void plScale( const plVector3 &scale)
{
    _plModelMatrixStack->scale( scale );
}

void plRotate(PLfloat angle_degree, PLfloat x, PLfloat y, PLfloat z )
{
    _plModelMatrixStack->rotate(angle_degree, plVector3(x,y,z) );
}

void plTranslate( PLfloat x, PLfloat y, PLfloat z )
{
    _plModelMatrixStack->translate( plVector3(x,y,z) );
}

void plScale( PLfloat x, PLfloat y, PLfloat z )
{
    _plModelMatrixStack->scale( plVector3(x,y,z) );
}

#endif
