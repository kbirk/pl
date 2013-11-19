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


namespace plShaderStack
{
    std::stack< const plShader* > _stack; 

    const plShader* _mapShader( PLuint shader )
    {
        static plVertexFragmentShader minimalShader( PL_FILE_PREPATH"shaders/minimal.vert", PL_FILE_PREPATH"shaders/minimal.frag" );       
        static plVertexFragmentShader phongShader  ( PL_FILE_PREPATH"shaders/phong.vert",   PL_FILE_PREPATH"shaders/phong.frag"   ); 
        static plVertexFragmentShader outlineShader( PL_FILE_PREPATH"shaders/outline.vert", PL_FILE_PREPATH"shaders/outline.frag"   );                 
        static plVertexFragmentShader textureShader( PL_FILE_PREPATH"shaders/texture.vert", PL_FILE_PREPATH"shaders/texture.frag" );  
        static plVertexFragmentShader fboShader    ( PL_FILE_PREPATH"shaders/fbo.vert",     PL_FILE_PREPATH"shaders/fbo.frag"     );

        switch( shader )
        {
            case PL_MINIMAL_SHADER:      return &minimalShader;
            case PL_PHONG_SHADER:        return &phongShader;
            case PL_OUTLINE_SHADER:      return &outlineShader;
            case PL_TEXTURE_2D_SHADER:   return &textureShader;   
            case PL_FBO_SHADER:          return &fboShader;         
        }

        return NULL;
    }

    void push( PLuint shader ) { _stack.push( _mapShader( shader ) ); }	
    void pop()                 { if ( !_stack.empty() ) _stack.pop(); }
    const plShader* top() { return _stack.top(); }
}


namespace plFrameBufferStack
{
    std::stack<const plFBO*> _stack; 

    void push( const plFBO& fbo ) 
    { 
        _stack.push( &fbo ); 
    }	
    
    
    void load( const plFBO& fbo )
    {
        if (_stack.size() > 0) 
        {
            _stack.top() = &fbo;
        } 
        else 
        {
            _stack.push( &fbo );	
        }  
    }
       
        
	void pop() 
	{ 
	    if ( !_stack.empty() ) 
	        _stack.pop(); 
	}
	
		  
	const plFBO* top()
	{
	    if ( _stack.empty() )
	        return NULL;
	        
	    return _stack.top();	
	}
}


namespace plTexture2DStack
{
    std::stack< const plTexture2D* > _stacks[ 32 ];   // limit number of texture units to hard cap of 32

    PLbool _checkUnitRange( PLuint unit )
    {
        if ( unit > plOpenGLInfo::maxTextureImageUnits )
        {
            std::cerr << "plTexture2DStack error: Attemping to use an texture image unit that the implementation doesn't support, max texture units = " 
                      << plOpenGLInfo::maxTextureImageUnits << std::endl;
            return false;
        }
        return true;
    }


    void push( const plTexture2D& texture, PLuint unit )
    {
        if ( !_checkUnitRange( unit ) )
            return;
            
        _stacks[ unit ].push( &texture );
    }	
    
    
    void load( const plTexture2D& texture, PLuint unit )
    {
        if ( !_checkUnitRange( unit ) )
            return;
            
        if (_stacks[ unit ].size() > 0) 
        {
            _stacks[ unit ].top() = &texture;
        } 
        else 
        {
            _stacks[ unit ].push( &texture );	
        } 
    }
    
    
	void pop( PLuint unit )
	{
	    if ( !_stacks[ unit ].empty() ) 
	        _stacks[ unit ].pop();
	}
	
	   
	const plTexture2D* top( PLuint unit )
	{
	    if ( _stacks[ unit ].empty() )
	        return NULL;
	        
	    return _stacks[ unit ].top();
	}
	
	
}       



