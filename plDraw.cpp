#include "plDraw.h"
 


void plDrawSetViewingMatrix()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    plMatrix44 viewingMatrix = _plCamera->matrix();
    glMultMatrixf( (GLfloat*)(&viewingMatrix) );
}


void plDrawSetProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
    plMatrix44 projectionMatrix = _plProjection->matrix();
    glMultMatrixf( (GLfloat*)(&projectionMatrix) );  
}


void _setOpenGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_DEPTH_TEST );   
}


void plDraw()
{
    _setOpenGLState();

    // projection
    plDrawSetProjectionMatrix();       
    // camera
    plDrawSetViewingMatrix();
    
    // picking draw
    _beginPicking();
    {
        // PLAN
        _plPlan->draw(true);
       
        // EDITORS
        if (_plGraftEditor != NULL)
            _plGraftEditor->draw();

        if (_plBoundaryEditor != NULL)
            _plBoundaryEditor->draw();
    }
    _endPicking();
    
    // normal draw
    _beginDrawing();
    {
        // PLAN
        _plPlan->draw();
       
        // EDITORS
        if (_plGraftEditor != NULL)
            _plGraftEditor->draw();

        //_plMinimalShader->bind(); 

        if (_plBoundaryEditor != NULL)
            _plBoundaryEditor->draw();
    }
    _endDrawing();
}

/*
void plSetShaderUniforms()
{
    if (isPicking)
    {
        _pickingShader.setPickingUniforms( pickingValue ); 
    }
    //_shader->setUniforms( PVM );
}
*/

void _beginPicking()
{
    //_shader = &_pickingShader;
    //isPicking = true;

    glDisable( GL_BLEND );

    // bind pickign shader and texture
    _plPickingTexture->bind();
    _plPickingShader->bind();

    // clear picking texture
    glClearColor(0,0,0,0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

}


void _endPicking()
{
    _plPickingTexture->unbind(); 
    _plPickingTexture->unbind(); 
    
    //_shader = NULL;
    //isPicking = false;
}


void _beginDrawing()
{
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  
    
    glClearColor( 1,1,1,0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //_shader = &_minimalShader;
    
    _plPhongShader->bind();   
    _plPhongShader->setLightUniform( plVector3(10, 10, 15) );
}


void _endDrawing()
{
    _plPhongShader->unbind();     
    //_shader = NULL;
}


void _drawScene()
{
    // PLAN
    _plPlan->draw();

    // EDITORS
    if (_plGraftEditor != NULL)
        _plGraftEditor->draw();

    if (_plBoundaryEditor != NULL)
        _plBoundaryEditor->draw();
}















/*

void _plSetOpenGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_DEPTH_TEST );    
}

void plDraw( PLbool clear_buffer, PLbool leave_shader_bound )
{
    _plSetOpenGLState();

    if (clear_buffer)
    {
        glClearColor( 1,1,1,0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }
    
    // PROJECTION
    plDrawSetProjectionMatrix();   
    
    // CAMERA
    plDrawSetViewingMatrix();
    
    // PICKING
    _plDrawPicking();
    
    // DRAW
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  
    
    _plPhongShader->bind();   
    _plPhongShader->setLightUniform( plVector3(10, 10, 15) );

    // PLAN
    _plPlan->draw();

    // EDITORS
    if (_plGraftEditor != NULL)
        _plGraftEditor->draw();

    if (_plBoundaryEditor != NULL)
        _plBoundaryEditor->draw();

    if (!leave_shader_bound) _plPhongShader->unbind();  
}

void plDrawUnbindShader()
{
    glUseProgram(0);
}

void plDrawScope(const plVector3 &pos, const plVector3 &rotAxis, PLfloat rotAngle, PLbool visible)
{
    // Draw the TA104 arthroscope
    static plMesh c1(1.5f, 2.0f, 120.0f, 16, 4);
    static plMesh c2(4.0f, 4.0f, 30.0f, 16, 4);
    static plMesh c3(8.0f, 8.0f, 60.0f, 16, 4);
    static plMesh c4(8.0f, 0.0f, 0.0f, 16, 4);

    if (visible)
        glColor3f(0.4,0.4,0.4);
    else
        glColor3f(1.0,0.25,0.05);

    glPushMatrix();
    {
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rotAngle,rotAxis.x,rotAxis.y,rotAxis.z);
        c1.draw();
        glPushMatrix();
        {
            glTranslatef(0, 0, 120);
            c2.draw();
            glPushMatrix();
            {
                glTranslatef(0,0,30);
                c3.draw();
                glTranslatef(0,0,60);
                c4.draw();
            }
            glPopMatrix();
        }    
        glPopMatrix();
    }
    glPopMatrix();
} 

void plDrawProbe(const plVector3 &pos, const plVector3 &rotAxis, PLfloat rotAngle, PLbool visible) 
{
    static plMesh c1(0.0f, 1.0f, 4.0f, 16, 4);
    static plMesh c2(1.0f, 2.5f, 124.0f, 16, 4);
    
    // Draw the TA002 probe
    if (visible)
        glColor3f(0.6,0.6,0.6);
    else
        glColor3f(1.0,0.3,0.1);
   
    glPushMatrix();
    {
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rotAngle,rotAxis.x,rotAxis.y,rotAxis.z);
        c1.draw();
        glPushMatrix();
        {
            glTranslatef(0, 0, 4);
            c2.draw();
        }
        glPopMatrix();
    }
    glPopMatrix();
}


void _plDrawPicking()
{
    glDisable( GL_BLEND );

    _plPickingTexture->bind();
    _plPickingShader->bind();
    
    glClearColor(0,0,0,0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    

   
    // PLAN
    _plPlan->draw(true);
   
    // EDITORS
    if (_plGraftEditor != NULL)
        _plGraftEditor->draw();

    if (_plBoundaryEditor != NULL)
        _plBoundaryEditor->draw();

    _plPickingTexture->unbind(); 
    _plPickingShader->unbind(); 
    
    
}
*/

void plDrawArrow( const plVector3 &origin, const plVector3 &direction, PLfloat length, PLfloat scale)
{
    glPushMatrix();
    {
        // set to origin
        glTranslatef( origin.x, origin.y, origin.z );
        plDrawArrow( direction, length, scale);
    }
    glPopMatrix();
}


void plDrawArrow( const plVector3 &direction, PLfloat length, PLfloat scale)
{
    plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());
       
    glPushMatrix();
    {               
        // set to direction
        glMultMatrixf( &rot(0,0) );       
        plDrawCylinder(scale*PL_HANDLE_RADIUS, length);
        glTranslatef( 0, 0, length );
        _plDrawArrowCone(scale);
        plDrawDisk(scale*PL_HEAD_RADIUS);
    } 
    glPopMatrix();   
}


void plDrawCircleArrow( const plVector3 &origin, const plVector3 &direction, PLfloat length, PLfloat scale)
{
    glPushMatrix();
    {
        // set to origin
        glTranslatef( origin.x, origin.y, origin.z );
        plDrawCircleArrow( direction, length, scale);
    }
    glPopMatrix();   
}


void plDrawCircleArrow(const plVector3 &direction, PLfloat length, PLfloat scale)
{
    plMatrix44 rot; rot.setRotation( plVector3(0,0,1), direction.normalize());
    
    glPushMatrix();
    {
        // set to direction
        glMultMatrixf( &rot(0,0) );
        plDrawCylinder(scale*PL_HANDLE_RADIUS, length);
        glTranslatef( 0, 0, length/scale );
        plDrawCylinder(scale*PL_HEAD_RADIUS, PL_CIRCLE_LENGTH);
        plDrawDisk(scale*PL_HEAD_RADIUS);
        glTranslatef( 0, 0, PL_CIRCLE_LENGTH );
        plDrawDisk(scale*PL_HEAD_RADIUS);
    } 
    glPopMatrix();     
}


void plDrawSphere(float radius)
{
    static plMesh sphere(1, 20, 20);

    glPushMatrix();
    {
        glScalef(radius, radius, radius);
        sphere.draw();
    }
    glPopMatrix();
}


void plDrawSphere(const plVector3 &origin, float radius)
{
    glPushMatrix();
    {
        glTranslatef( origin.x, origin.y, origin.z );
        plDrawSphere( radius );
    }
    glPopMatrix();
}


void plDrawCylinder(float radius, float height)
{
    static plMesh cylinder(1.0f, 1.0f, 1.0f, 30, 1);    

    glPushMatrix();
    {
        glScalef(radius, radius, height);        
        cylinder.draw();
    }
    glPopMatrix();
}

void plDrawDisk(const plVector3 &origin, float radius)
{
    glPushMatrix();
    {
        glTranslatef( origin.x, origin.y, origin.z );
        plDrawDisk( radius );       
    }
    glPopMatrix();
}

void plDrawDisk(float radius)
{
    static plMesh disk(0.0f, 1.0f, 20, 20);   

    glPushMatrix();
    {
        glScalef(radius, radius, radius);        
        disk.draw();
    }
    glPopMatrix();
}


void _plDrawArrowCone(float scale)
{
    static plMesh cone(PL_HEAD_RADIUS, 0.0f, PL_ARROW_LENGTH, 30, 1);
    
    glPushMatrix();
    {
        glScalef(scale, scale, scale);        
        cone.draw();
    }
    glPopMatrix();
}


