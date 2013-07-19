#include "plRenderer.h"

// plRenderer static variables
const plPlan*            plRenderer::_planToDraw             = NULL;
const plGraftEditor*     plRenderer::_graftEditorToDraw      = NULL;
const plBoundaryEditor*  plRenderer::_boundaryEditorToDraw   = NULL;
const plTextureMesh*     plRenderer::_arthroTextureToDraw     = NULL;
plSeq<const plTrackedObject*> plRenderer::_trackedObjectsToDraw;
plSeq<const plLineMesh*>      plRenderer::_debugToDraw;

plComputeShader*         plRenderer::_computeShader          = NULL;
plMinimalShader*         plRenderer::_minimalShader          = NULL;
plPhongShader*           plRenderer::_phongShader            = NULL; 
plPickingShader*         plRenderer::_pickingShader          = NULL;
plTextureShader*         plRenderer::_textureShader          = NULL;


void plRenderer::init()
{
    //_computeShader = new plComputeShader("./shaders/test.comp");
    _minimalShader = new plMinimalShader("./shaders/minimal.vert", "./shaders/minimal.frag");
    _phongShader   = new plPhongShader  ("./shaders/phong.vert", "./shaders/phong.frag");
    _pickingShader = new plPickingShader("./shaders/picking.vert", "./shaders/picking.frag");  
    _textureShader = new plTextureShader("./shaders/texture.vert", "./shaders/texture.frag");  

    plPicking::init(1,1);
    plProjectionStack::load( plProjection( 7.0f, 1.6f, 100.0f, 15000.0f) );
} 


void plRenderer::_clearRenderQueue()
{
    _planToDraw           = NULL;
    _graftEditorToDraw    = NULL;
    _boundaryEditorToDraw = NULL;
    _arthroTextureToDraw  = NULL;
    _trackedObjectsToDraw.clear();
    _debugToDraw.clear();
}


void plRenderer::queue ( const plPlan &plan )
{
    if (_planToDraw != NULL)
        std::cerr << "plRenderer queue() error: plPlan already queued top draw, overridding previous \n";
        
    _planToDraw = &plan;
}


void plRenderer::queue( const plGraftEditor &editor )
{
    if (_boundaryEditorToDraw != NULL)
        std::cerr << "plRenderer queue() error: plGraftEditor already queued top draw, overridding previous \n";
        
    _graftEditorToDraw = &editor;
}


void plRenderer::queue( const plBoundaryEditor &editor )
{
    if (_boundaryEditorToDraw != NULL)
        std::cerr << "plRenderer queue() error: plBoundaryEditor already queued top draw, overridding previous \n";
        
    _boundaryEditorToDraw = &editor;
}


void plRenderer::queue( const plTextureMesh &arthroTexture )
{
    if (_arthroTextureToDraw != NULL)
        std::cerr << "plRenderer queue() error: plTextureMesh already queued top draw, overridding previous \n";
        
    _arthroTextureToDraw = &arthroTexture;
}


void plRenderer::queue( const plTrackedObject &object )
{
    _trackedObjectsToDraw.add( &object );
}


void plRenderer::queue( const plLineMesh &debug )
{
    _debugToDraw.add( &debug );
}


void plRenderer::reportError( const plString &str  ) 
{
    GLuint errnum;
    const char *errstr;
    while (errnum = glGetError()) 
    {
        errstr = reinterpret_cast<const char *> (gluErrorString(errnum));
        std::cout << str << " " << errstr << "\n";
    }
}


void plRenderer::_setOpenGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_DEPTH_TEST );   
}


void plRenderer::draw()
{
    _setOpenGLState();

    // picking draw    
    _beginPicking();
    {
        _drawScenePicking();
    }
    _endPicking();
        
    // normal draw
    _beginDrawing();
    {
        _drawScene();
    }

    _endDrawing();

    // clear queues from this frame
    _clearRenderQueue();
}


void plRenderer::_beginPicking()
{
    glDisable( GL_BLEND );

    // bind picking shader
    plShaderStack::push( _pickingShader );
    
    // bind picking texture
    plPicking::texture->bind();

    // clear picking texture
    glClearColor(0,0,0,0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

}


void plRenderer::_endPicking()
{
    plShaderStack::pop();
    plPicking::texture->unbind();  
}


void plRenderer::_beginDrawing()
{
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  
    
    glClearColor( 1,1,1,0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    plShaderStack::push( _phongShader );     
}


void plRenderer::_endDrawing()
{
    plShaderStack::pop(); 
}


void plRenderer::_drawScene()
{

    // arthro texture
    if (_arthroTextureToDraw != NULL)
    {
        _drawArthroTexture();
    }
    
    // plan
    if (_planToDraw != NULL)
    {
        _planToDraw->drawElements();
        _planToDraw->drawModels();
    }

    // graft editor
    if (_graftEditorToDraw != NULL)
    {
        _graftEditorToDraw->draw();
    }
    
    // boundary editor
    if (_boundaryEditorToDraw != NULL && _planToDraw != NULL)
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        
        // set flat shader
        plShaderStack::push( _minimalShader );
        // set identity viewing matrix and ortho projection to screen dimension   
        plCameraStack::push( plMatrix44() );    
        plProjectionStack::push( plMatrix44( 0, viewport[2], 0, viewport[3], -1, 1) ); // ortho, viewport dimensions
        
        _boundaryEditorToDraw->draw( *_planToDraw );
        
        plCameraStack::pop(); 
        plProjectionStack::pop();
        
        plShaderStack::pop();   
    }   
    
    // tracked objects
    for (PLuint i=0; i<_trackedObjectsToDraw.size(); i++)
    {
        if (_trackedObjectsToDraw[i]->isArthroscope())
        {
            plDraw::scope( *_trackedObjectsToDraw[i] );
        }    
        else
        {
            plDraw::probe( *_trackedObjectsToDraw[i] );
        }
    }
  
    // automatic planner
    for (PLuint i=0; i<plAutomaticPlanner::_donorSiteGrids.size(); i++)
    {
        plColourStack::load( 0.9, 0.6, 0.2 );
        for (PLuint j=0; j<plAutomaticPlanner::_donorSiteGrids[i].points.size(); j++)
        {
            plDraw::sphere( plAutomaticPlanner::_donorSiteGrids[i].points[j], 0.10f );
            
        }
    }
    
    /*
    // set flat shader
    plShaderStack::push( _minimalShader );    
    
    // octrees objects
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    
    for (PLuint i =0; i < _planToDraw->models().size(); i++)
    {      
        plColourStack::load( 0.3, 0.1, 0.4 );
        //_planToDraw->models(i).bone.octree().draw();
        //plColourStack::load( 0.1, 0.4, 0.3 );
        _planToDraw->models(i).cartilage.octree().draw();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // debug objects
    for (PLuint i=0; i<_debugToDraw.size(); i++)
    {
        _debugToDraw[i]->draw();
    }
    plShaderStack::pop(); 
    */

}


void plRenderer::_drawArthroTexture()
{
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );

    PLfloat xmargin = (viewport[2] / viewport[3] - 1) / 2.0;
    
    plShaderStack::push( _textureShader );
    
    // ortho projection
    plMatrix44 ortho( 0, 1, 0, 1, -1, 1);
    plProjectionStack::push( ortho ); 

    // identity model matrix
    plModelStack::push( plMatrix44() );
    
    // default camera matrix
    plMatrix44 camera( 1, 0,  0, 0,
                       0, 1,  0, 0,
                       0, 0, -1, 0,
                       0, 0,  0, 1 );        
    plCameraStack::push( camera );

    glDisable( GL_DEPTH_TEST );

    _arthroTextureToDraw->draw();
     
    glEnable( GL_DEPTH_TEST ); 
           
    plProjectionStack::pop(); 
    plModelStack::pop();       
    plCameraStack::pop();
    
    plShaderStack::pop();
}


void plRenderer::_drawScenePicking()
{
    // plan
    if (_planToDraw != NULL)
    {       
        glEnable     ( GL_STENCIL_TEST );				      // need stencil testing enabled  					
	    glStencilFunc( GL_ALWAYS, 0xFF, 0xFF );               // replace where rendered 		
	    glStencilOp  ( GL_REPLACE, GL_REPLACE, GL_REPLACE );  // always replace previous bit	
	    glStencilMask( 0x01 );                                // only write to first bit
       
        // elements
        _planToDraw->drawElements();

        glStencilOp  ( GL_KEEP, GL_KEEP, GL_KEEP );	        // no longer modify the stencil bits  
        glStencilFunc( GL_EQUAL, 0x00, 0xFF);               // only render to bits = 0 (have not been written)

        // models
        _planToDraw->drawModels();

        glDisable( GL_STENCIL_TEST );                       // no more stencil testing
    }

    // graft editor
    if (_graftEditorToDraw != NULL)
    {
        _graftEditorToDraw->draw();
    }
    
    // boundary editor
    if (_boundaryEditorToDraw != NULL && _planToDraw != NULL)
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        
        // set identity viewing matrix and ortho projection to screen dimension
        plCameraStack::push( plMatrix44() );    
        plProjectionStack::push( plMatrix44( 0, viewport[2], 0, viewport[3], -1, 1) ); // ortho, viewport dimensions
        
        _boundaryEditorToDraw->draw( *_planToDraw );
        
        plCameraStack::pop(); 
        plProjectionStack::pop();         
    }   

}

