#include "plRenderingPipeline.h"

// plColourStack
plStack<plVector4>  plColourStack::_stack;
PLbool              plColourStack::updated = false;

// plModelStack static variables
plMatrixStack plModelStack::_stack;
PLbool        plModelStack::updated = false;

// plProjectionStack static variables
plMatrixStack plProjectionStack::_stack;
PLbool        plProjectionStack::updated = false;

// plCameraStack static variables
plMatrixStack plCameraStack::_stack;
PLbool        plCameraStack::updated = false;	

// plShaderStack static variables
plStack<const plShader*>   plShaderStack::_stack; 


void plColourStack::push( PLfloat r, PLfloat g, PLfloat b, PLfloat a)
{ 
    _stack.push( plVector4(r,g,b,a) );  
    updated = true; 
}

	
void plColourStack::load( PLfloat r, PLfloat g, PLfloat b, PLfloat a)	  
{ 
    if (_stack.size() > 0) 
    {
        _stack.top() = plVector4(r,g,b,a);
    } 
    else 
    {
        _stack.push(plVector4(r,g,b,a));	
    }   
    updated = true; 
}


void plColourStack::pop()                                                         
{ 
    _stack.pop();
    updated = true; 
}


const plVector4& plColourStack::top()		                                 
{ 
    return _stack.top(); 
} 


plVector3 plCameraStack::direction()
{
    return plVector3(-plCameraStack::top()[2], -plCameraStack::top()[6], -plCameraStack::top()[10]);     
}


plVector3 plCameraStack::position()
{
    // get rotation matrix
    plMatrix44 rot = _stack.top();   rot.setColumn(3, 0, 0, 0, 1);  // remove translation component
        
    plVector3 d(-plCameraStack::top()[12], -plCameraStack::top()[13], -plCameraStack::top()[14]);
    
    return d * rot;    
}
	
plVector3 plCameraStack::up()
{
    // get rotation matrix
    plMatrix44 rot = _stack.top();   rot.setColumn(3, 0, 0, 0, 1);  // remove translation component
        
    plVector3 d(-plCameraStack::top()[4], -plCameraStack::top()[5], -plCameraStack::top()[6]);
    
    return d * rot;
}		

void plShaderStack::push( const plShader *shader )  
{ 
    _stack.push(shader); 
    _stack.top()->bind(); 
}	

void plShaderStack::pop()                          
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
   
void plShaderStack::use()
{
    if ( dynamic_cast<const plPickingShader*>( _stack.top() ) )
    {
        // picking shader
        const plPickingShader *shader = static_cast<const plPickingShader*>(_stack.top());
        shader->setPickingUniforms( plPicking::value );
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
        const plMinimalShader *shader = static_cast<const plMinimalShader*>(_stack.top());
        shader->setColourUniform( plColourStack::top()  );
    }  
    else if ( dynamic_cast<const plTextureShader*>( _stack.top() ) )
    {
        // texture shader
        const plTextureShader *shader = static_cast<const plTextureShader*>(_stack.top());
        shader->setTextureUniform();
    }  

    if (plModelStack::updated || plCameraStack::updated || plProjectionStack::updated)
    {
        // only set transform uniforms if they need to be changed
        _stack.top()->setTransformUniforms( plModelStack::top(), plCameraStack::top(), plProjectionStack::top() );
        // reset update flags
        plModelStack::updated      = false;
        plCameraStack::updated     = false;
        plProjectionStack::updated = false;
    }

}   

////////////////////////////////////////////////////////

