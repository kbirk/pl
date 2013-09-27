#include "plRenderingPipeline.h"


namespace plColourStack
{
    plStack<plVector4>  _stack;
           
    void push( PLfloat r, PLfloat g, PLfloat b, PLfloat a) {  _stack.push( plVector4(r,g,b,a) );  }
    
    void pop () { _stack.pop(); } 
       
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

}


namespace plModelStack
{  		
    plMatrixStack _stack;

	void push()				         { _stack.push(_stack.top());      }	
	void push( const plMatrix44 &m ) { _stack.push(m);			       }	
	void mult( const plMatrix44 &m ) { _stack.load( _stack.top() * m); } 
	void pop()                       { _stack.pop();                   }
				
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
	void pop()                         { _stack.pop();               } 
	
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
	void pop()                         { _stack.pop();               }
    
    const plMatrix44& top()	           { return _stack.top();        } 

    plVector3 direction()
    {
        return plVector3(-top()[2], -top()[6], -top()[10]);     
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
    plStack<const plShader*>   _stack; 


    void push( const plShader *shader )  
    { 
        _stack.push(shader); 
        _stack.top()->bind(); 
    }	


    void pop()                          
    { 
        _stack.pop(); 
        if (_stack.size() == 0) 
        {
            glUseProgram(0);
        }     
        else 
        {
            _stack.top()->bind();
        }    
    }


    void use()
    {
        if ( dynamic_cast<const plPickingShader*>( _stack.top() ) )
        {
            // picking shader
            const plPickingShader *shader = static_cast<const plPickingShader*>(_stack.top());
            shader->setPickingUniforms( plPicking::value.type, plPicking::value.id, plPicking::value.index );
        }
        else if ( dynamic_cast<const plPhongShader*>( _stack.top() ) )
        {
            // phong shader
            const plPhongShader *shader = static_cast<const plPhongShader*>(_stack.top());
            shader->setLightUniform ( plVector3(10, 10, 15) );
            shader->setColourUniform( plColourStack::top()  );
        }  
        else if ( dynamic_cast<const plMinimalShader*>( _stack.top() ) )
        {
            // minimal shader
            const plMinimalShader *shader = static_cast<const plMinimalShader*>( _stack.top() );
            shader->setColourUniform( plColourStack::top()  );
        }  
        else if ( dynamic_cast<const plTextureShader*>( _stack.top() ) )
        {
            // texture shader
            const plTextureShader *shader = static_cast<const plTextureShader*>(_stack.top());
            shader->setTextureUniform();
        }  

        const plMinimalShader *shader = static_cast<const plMinimalShader*>( _stack.top() );
        shader->setTransformUniforms( plModelStack::top(), plCameraStack::top(), plProjectionStack::top() );

    }   
}


