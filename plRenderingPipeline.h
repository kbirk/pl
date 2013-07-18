#ifndef __PL_RENDERING_PIPELINE__
#define __PL_RENDERING_PIPELINE__

#include "plCommon.h"
#include "plVector4.h"
#include "plMatrixStack.h"
#include "plProjection.h"
#include "plCamera.h"
#include "plPickingShader.h"
#include "plMinimalShader.h"
#include "plPhongShader.h"

class plColourStack
{
    public:

        static PLbool updated;

        static void push( PLfloat r, PLfloat g, PLfloat b, PLfloat a = 1.0f );	
		static void load( PLfloat r, PLfloat g, PLfloat b, PLfloat a = 1.0f );
		static void pop ();
		
        static const plVector4& top();

    private:

        plColourStack() {};

        static plStack<plVector4> _stack;
};


class plModelStack
{
    public:
           		
		static PLbool updated;

		static void push()				          { _stack.push(_stack.top());       updated = true; }	
		static void push( const plMatrix44 &m )	  { _stack.push(m);			         updated = true; }	
		static void mult( const plMatrix44 &m )   { _stack.load( _stack.top() * m) ; updated = true; } 
		static void pop()                         { _stack.pop();                    updated = true; }
				
		static void scale    ( GLfloat x, GLfloat y, GLfloat z)                 { _stack.scale(x,y,z);        updated = true; }		
		static void translate( GLfloat x, GLfloat y, GLfloat z)			        { _stack.translate(x,y,z);    updated = true; }   
		static void rotate   ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z)  { _stack.rotate(angle,x,y,z); updated = true; }
        
		static void scale    ( const plVector3 &v )                             { _stack.scale(v);         updated = true; }						
		static void translate( const plVector3 &v )                             { _stack.translate(v);     updated = true; }	
		static void rotate   ( float angle, const plVector3 &v)                 { _stack.rotate(angle, v); updated = true; }

        static const plMatrix44& top()		  { return _stack.top();} 
    
    private:
        
        plModelStack() {};

        static plMatrixStack _stack;
    
};


class plProjectionStack
{
    
    public:
    
        static PLbool updated;
           
        static void push()				          { _stack.push();              updated = true; }	
		static void push( const plMatrix44   &m ) { _stack.push(m);             updated = true; }	
		static void push( const plProjection &p ) { _stack.push(p.matrix());    updated = true; }	
		static void load( const plMatrix44   &m ) { _stack.load(m);             updated = true; }	
		static void load( const plProjection &p ) { _stack.load(p.matrix());    updated = true; }
		static void pop()                         { _stack.pop();               updated = true; } 
        static const plMatrix44& top()	      { return _stack.top(); } 
    
    private:
        
        plProjectionStack() {}

        static plMatrixStack _stack;
};


class plCameraStack
{
    
    public:
    
        static PLbool updated;

        static void push()				          { _stack.push();           updated = true; }	
		static void push( const plMatrix44 &m )	  { _stack.push(m);          updated = true; }	
		static void push( const plCamera &c )     { _stack.push(c.matrix()); updated = true; }	 
		static void load( const plMatrix44 &m )	  { _stack.load(m);          updated = true; }	
		static void load( const plCamera &c )     { _stack.load(c.matrix()); updated = true; }	
		static void pop()                         { _stack.pop();            updated = true; }
		
		static plVector3 direction();
		static plVector3 position();                   
		static plVector3 up();
		
        static const plMatrix44& top()	      { return _stack.top();} 
    
    private:
                  
        plCameraStack() {}

        static plMatrixStack _stack;
};


class plShaderStack
{   
    public:

		static void push( const plShader *shader ) ;		
		static void pop();           
        static void use();  
           
    private:
    
        plShaderStack() {};

        static plStack<const plShader*> _stack;
  
};

#endif
