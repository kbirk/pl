#include "plRenderingPipeline.h"


namespace plColourStack
{
    std::stack<plVector4>  _stack;
           
    void push( PLfloat r, PLfloat g, PLfloat b, PLfloat a) {  _stack.push( plVector4(r,g,b,a) );  }
    void push( const plVector4 &colour ) { _stack.push( colour ); }
    void pop () { if ( !_stack.empty() ) _stack.pop(); } 
       
	const plVector4& top()	{ return _stack.top(); } 
	
    void load( PLfloat r, PLfloat g, PLfloat b, PLfloat a)	  
    { 
        if (_stack.size() > 0) 
        {
            _stack.top() = plVector4(r,g,b,a);
        } 
        else 
        {
            _stack.push( plVector4(r,g,b,a) );	
        }   
    }

	void load( const plVector4 &colour )
	{
	    load( colour.x, colour.y, colour.z, colour.w );
	}

}


namespace plPickingStack
{
    std::stack<PLuint>  _redStack;
    std::stack<PLuint>  _greenStack;
    std::stack<PLuint>  _blueStack;
      
    void pushRed   ( PLint r ) { _redStack.push( r );   }
    void pushGreen ( PLint g ) { _greenStack.push( g ); }
    void pushBlue  ( PLint b ) { _blueStack.push( b );  }
    
	void loadRed   ( PLint r ) 
	{ 
	    if (_redStack.size() > 0) 
        {
            _redStack.top() = r;
        } 
        else 
        {
            _redStack.push( r );	
        }  
    }
    
    
	void loadGreen ( PLint g )
	{ 
	    if (_greenStack.size() > 0) 
        {
            _greenStack.top() = g;
        } 
        else 
        {
            _greenStack.push( g );	
        }  
    }
    
    
	void loadBlue  ( PLint b )
	{ 
	    if (_blueStack.size() > 0) 
        {
            _blueStack.top() = b;
        } 
        else 
        {
            _blueStack.push( b );	
        }  
    }
	
	
	void popRed   () { if ( !_redStack.empty()   ) _redStack.pop();   }
	void popGreen () { if ( !_greenStack.empty() ) _greenStack.pop(); }
	void popBlue  () { if ( !_blueStack.empty()  ) _blueStack.pop();  }
	
	PLint topRed()   { return _redStack.top(); }
	PLint topGreen() { return _greenStack.top(); }
	PLint topBlue()  { return _blueStack.top(); }
	
    plPickingInfo top() { return plPickingInfo( _redStack.top(), _greenStack.top(), _blueStack.top() );  }
}



namespace plModelStack
{  		
    plMatrixStack _stack;

	void push()				         { _stack.push(_stack.top());      }	
	void push( const plMatrix44 &m ) { _stack.push(m);			       }	
	void mult( const plMatrix44 &m ) { _stack.load( _stack.top() * m); } 
	void pop()                       { if ( !_stack.empty() ) _stack.pop(); }
	
	void scale    ( GLfloat s )                                      { _stack.scale(s,s,s);        }			
	void scale    ( GLfloat x, GLfloat y, GLfloat z)                 { _stack.scale(x,y,z);        }		
	void translate( GLfloat x, GLfloat y, GLfloat z)			     { _stack.translate(x,y,z);    }   
	void rotate   ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z)  { _stack.rotate(angle,x,y,z); }
    
	void scale    ( const plVector3 &v )                             { _stack.scale(v);         }						
	void translate( const plVector3 &v )                             { _stack.translate(v);     }	
	void rotate   ( float angle, const plVector3 &v)                 { _stack.rotate(angle, v); }
	
	const plMatrix44& top()		     { return _stack.top();            } 
}


namespace plProjectionStack
{  
    plMatrixStack _stack;
  
    void push()				           { _stack.push();              }	
	void push( const plMatrix44   &m ) { _stack.push(m);             }	
	void push( const plProjection &p ) { _stack.push(p.matrix());    }	
	void load( const plMatrix44   &m ) { _stack.load(m);             }	
	void load( const plProjection &p ) { _stack.load(p.matrix());    }
	void pop()                         { if ( !_stack.empty() ) _stack.pop(); } 
	
    const plMatrix44& top()	           { return _stack.top();        } 
}


namespace plCameraStack
{
    plMatrixStack _stack;

    void push()				           { _stack.push();              }	
	void push( const plMatrix44 &m )   { _stack.push(m);             }	
	void push( const plCamera   &c )   { _stack.push(c.getMatrix()); }	 
	void load( const plMatrix44 &m )   { _stack.load(m);             }	
	void load( const plCamera   &c )   { _stack.load(c.getMatrix()); }	
	void pop()                         { if ( !_stack.empty() ) _stack.pop(); }
    
    const plMatrix44& top()	           { return _stack.top();        } 

    plVector3 direction()
    {
        return plVector3( -top()[2], -top()[6], -top()[10] );     
    }


    plVector3 position()
    {
        // get rotation matrix
        plMatrix44 rot = _stack.top();   rot.setColumn(3, 0, 0, 0, 1);  // remove translation component
            
        plVector3 d(-top()[12], -top()[13], -top()[14]);
        
        return d * rot;    
    }
	
	
    plVector3 up()
    {
        // get rotation matrix
        plMatrix44 rot = _stack.top();   rot.setColumn(3, 0, 0, 0, 1);  // remove translation component
            
        plVector3 d(-top()[4], -top()[5], -top()[6]);
        
        return d * rot;
    }
    		
}


