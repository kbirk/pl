#ifndef __PL_MATRIX_STACK__
#define __PL_MATRIX_STACK__

#include "plCommon.h"
#include "plMatrix44.h"

class plMatrixStack
{

	public:
		
		plMatrixStack();

        void load( const plMatrix44 &m );
		void push();
		void push( const plMatrix44 &m );
		void mult( const plMatrix44 &m );
		void pop();
				
		void scale    (GLfloat x, GLfloat y, GLfloat z);		
		void translate(GLfloat x, GLfloat y, GLfloat z);			
		void rotate   (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
        
		void scale    (const plVector3 &v);				
		void translate(const plVector3 &v);			
		void rotate   (float angle, const plVector3 &v);		

        const plMatrix44& top(void);

	private:

		plStack<plMatrix44> _stack;

};


#endif
