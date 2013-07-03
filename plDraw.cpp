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
    
    // NORMAL DRAW
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  
    
    _plMinimalShader->bind();   
    _plMinimalShader->setLightUniform( plVector3(10, 10, 15) );
    
    // PLAN
    _plPlan->draw();
    
    // MODEL
    for (PLuint i =0; i < _plBoneAndCartilageModels.size(); i++)
    {        
        _plBoneAndCartilageModels[i]->draw();
    }

    if (!leave_shader_bound) _plMinimalShader->unbind();  
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
    
    /*
    // PROJECTION
    plDrawSetProjectionMatrix();   
    
    // CAMERA
    plDrawSetViewingMatrix
    */   
    
    // stencil testing used to allow user to click on plan components through transparent bone/cartilage
	glEnable( GL_STENCIL_TEST );						// need stencil testing enabled  					
	glStencilFunc( GL_ALWAYS, 0xFF, 0xFF );             // replace where rendered 		
	glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );	// always replace previous bit	
	glStencilMask(0x01);                                // only write to first bit
        
    // PLAN
    _plPlan->draw();
        
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );	        // no longer modify the stencil bits  
    glStencilFunc( GL_EQUAL, 0x00, 0xFF);               // only render to bits = 0 (have not been written)
    
    // MODEL
    for (PLuint i =0; i < _plBoneAndCartilageModels.size(); i++)
    {            
        _plPickingState->id = i;    
        _plBoneAndCartilageModels[i]->draw();
    }
    
    _plPickingTexture->unbind(); 
    _plPickingShader->unbind(); 
    
    glDisable( GL_STENCIL_TEST );
}


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
    glPushMatrix();
    {
        glScalef(radius, radius, radius);
        _plSphere->draw();
    }
    glPopMatrix();
}


void plDrawSphere(const plVector3 &origin, float radius)
{
    glPushMatrix();
    {
        glTranslatef( origin.x, origin.y, origin.z );
        glScalef(radius, radius, radius);
        _plSphere->draw();
    }
    glPopMatrix();
}


void plDrawCylinder(float radius, float height)
{
    glPushMatrix();
    {
        glScalef(radius, radius, height);        
        _plCylinder->draw();
    }
    glPopMatrix();
}

void plDrawDisk(float radius)
{
    glPushMatrix();
    {
        glScalef(radius, radius, radius);        
        _plDisk->draw();
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


