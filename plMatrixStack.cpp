#include "plMatrixStack.h"

plMatrixStack::plMatrixStack()
{
	// stack always starts with identity matrix
	_stack.push( plMatrix44() );
}

void plMatrixStack::load( const plMatrix44 &m )  
{
    if (_stack.size() == 0)
    {
        _stack.push(m); 
    }
    else 
    {
        _stack.top() = m; 
    }
}

void plMatrixStack::push()
{ 
    if (_stack.size() == 0) 
    {
        _stack.push(plMatrix44()); // push identity 
    }
    else 
    {
        _stack.push(_stack.top()); 
    }
}	

void plMatrixStack::push( const plMatrix44 &m )  
{ 
    _stack.push(m);
}	

void plMatrixStack::mult( const plMatrix44 &m )  
{
    _stack.top() = _stack.top() * m; 
} 

void plMatrixStack::pop()
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
		
const plMatrix44& plMatrixStack::top(void)		
{ 
     return _stack.top(); 
}

void plMatrixStack::scale(GLfloat x, GLfloat y, GLfloat z)
{
	plMatrix44 scale;
	scale.setScale(x, y, z);
	_stack.top() = _stack.top() * scale;
}	


void plMatrixStack::translate(GLfloat x, GLfloat y, GLfloat z)
{
	plMatrix44 trans;
	trans.setTranslation(x, y, z);
	_stack.top() = _stack.top() * trans;	
}


void plMatrixStack::rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	plMatrix44 rot;
	rot.setRotationD(angle, plVector3(x, y, z) );
	_stack.top() = _stack.top() * rot;	
}

       
void plMatrixStack::scale(const plVector3 &v) 
{
	plMatrix44 scale;
	scale.setScale(v);
	_stack.top() = _stack.top() * scale;
}	


void plMatrixStack::translate(const plVector3 &v) 
{
	plMatrix44 trans;
	trans.setTranslation(v);
	_stack.top() = _stack.top() * trans;		
}	


void plMatrixStack::rotate(float angle, const plVector3 &v)
{
	plMatrix44 rot;
	rot.setRotationD(angle, v);
	_stack.top() = _stack.top() * rot;	
}				
