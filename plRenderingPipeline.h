#ifndef PL_RENDERING_PIPELINE
#define PL_RENDERING_PIPELINE

#include "plCommon.h"
#include "plVector4.h"
#include "plMatrixStack.h"
#include "plProjection.h"
#include "plCamera.h"
#include "plPicking.h"


namespace plColourStack
{
    void push( PLfloat r, PLfloat g, PLfloat b, PLfloat a = 1.0f );
    void push( const plVector4 &colour );
	void load( PLfloat r, PLfloat g, PLfloat b, PLfloat a = 1.0f );
	void load( const plVector4 &colour );
	void pop ();
	
    const plVector4& top();
}


namespace plPickingStack
{
    void pushRed   ( PLint r );
    void pushGreen ( PLint g );
    void pushBlue  ( PLint b );
	
	void loadRed   ( PLint r );
	void loadGreen ( PLint g );
	void loadBlue  ( PLint b );
	
	void popRed   ();
	void popGreen ();
	void popBlue  ();
	
	PLint topRed();
	PLint topBlue();
	PLint topGreen();
	
    plPickingInfo top();
}


namespace plModelStack
{  		
	void push();
	void push( const plMatrix44 &m );
	void mult( const plMatrix44 &m );
	void pop();
	
	void scale    ( GLfloat s );		
	void scale    ( GLfloat x, GLfloat y, GLfloat z);		
	void translate( GLfloat x, GLfloat y, GLfloat z);
	void rotate   ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    
	void scale    ( const plVector3 &v );			
	void translate( const plVector3 &v );
	void rotate   ( float angle, const plVector3 &v);

    const plMatrix44& top();
}


namespace plCameraStack
{
    void push();
	void push( const plMatrix44 &m );
	void push( const plCamera   &c );
	void load( const plMatrix44 &m );
	void load( const plCamera   &c );
	void pop();
	
	plVector3 direction();
	plVector3 position();                   
	plVector3 up();
	
    const plMatrix44& top();
}


namespace plProjectionStack
{    
    void push();
	void push( const plMatrix44   &m );
	void push( const plProjection &p );
	void load( const plMatrix44   &m );
	void load( const plProjection &p );
	void pop();
	
    const plMatrix44& top();
}


#endif
