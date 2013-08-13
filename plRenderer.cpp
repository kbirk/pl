#include "plRenderer.h"

// plRenderer static variables
const plPlan*            plRenderer::_planToDraw             = NULL;
const plGraftEditor*     plRenderer::_graftEditorToDraw      = NULL;
const plBoundaryEditor*  plRenderer::_boundaryEditorToDraw   = NULL;
const plTextureMesh*     plRenderer::_arthroTextureToDraw    = NULL;
plSeq<const plTrackedObject*> plRenderer::_trackedObjectsToDraw;

plMinimalShader*         plRenderer::_minimalShader          = NULL;
plPhongShader*           plRenderer::_phongShader            = NULL; 
plPickingShader*         plRenderer::_pickingShader          = NULL;
plTextureShader*         plRenderer::_textureShader          = NULL;


void plRenderer::init()
{
    _minimalShader = new plMinimalShader("./shaders/minimal.vert", "./shaders/minimal.frag");
    _phongShader   = new plPhongShader  ("./shaders/phong.vert", "./shaders/phong.frag");
    _pickingShader = new plPickingShader("./shaders/picking.vert", "./shaders/picking.frag");  
    _textureShader = new plTextureShader("./shaders/texture.vert", "./shaders/texture.frag");  

    plPicking::init(1,1);
} 


void plRenderer::_clearRenderQueue()
{
    _planToDraw           = NULL;
    _graftEditorToDraw    = NULL;
    _boundaryEditorToDraw = NULL;
    _arthroTextureToDraw  = NULL;
    _trackedObjectsToDraw.clear();
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


void plRenderer::reportError( const plString &str  ) 
{
    GLuint errnum;
    const char *errstr;
    while (errnum = glGetError()) 
    {
        errstr = reinterpret_cast<const char*> (gluErrorString(errnum));
        std::cout << str << " " << errstr << "\n";
    }
}


void plRenderer::_setOpenGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_DEPTH_TEST );   
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  
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
    
    glClearColor( 1,1,1,0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glStencilFunc( GL_ALWAYS, 0x00, 0x00); // always render
    
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

    // graft editor handles
    if (_graftEditorToDraw != NULL)
    {
        _graftEditorToDraw->drawHandles();
    }
    
    // draw editor menus
    if ( (_boundaryEditorToDraw != NULL || _graftEditorToDraw != NULL) && _planToDraw != NULL)
    {
        // set flat shader
        plShaderStack::push( _minimalShader );  
        plCameraStack::push( plMatrix44() );                                                        // identity viewing matrix
        plProjectionStack::push( plMatrix44( 0, plWindow::width(), 0, plWindow::height(), -1, 1) ); // ortho, viewport dimensions
        {
            _boundaryEditorToDraw->drawMenu( *_planToDraw );
            _graftEditorToDraw->drawMenu( *_planToDraw );
        }
        plProjectionStack::pop(); 
        plCameraStack::pop();                
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
  
    // DEBUG
    // GRID POINTS
    for (PLuint i=0; i<plAutomaticPlanner::_donorSiteGrids.size(); i++)
    {
        plColourStack::load( 0.9, 0.6, 0.2 );
        for (PLuint j=0; j<plAutomaticPlanner::_donorSiteGrids[i].size(); j++)
        {
            PLfloat x = plAutomaticPlanner::_donorSiteGrids[i].points(j).x;
            PLfloat y = plAutomaticPlanner::_donorSiteGrids[i].points(j).y;
            PLfloat z = plAutomaticPlanner::_donorSiteGrids[i].points(j).z;
            plDraw::sphere( plVector3(x,y,z), 0.09f );
            
        }
    }
    
    // DEBUG
    // POTENTIAL GRAFTS
    for (PLuint i=0; i<plAutomaticPlanner::_defectSiteGrids.size(); i++)
    {    
        plColourStack::load( 0.2, 0.6, 0.9 );
        
        for (PLuint j=0; j<plAutomaticPlanner::_defectSiteGrids[i].size(); j++)
        {
            PLfloat x = plAutomaticPlanner::_defectSiteGrids[i].points(j).x;
            PLfloat y = plAutomaticPlanner::_defectSiteGrids[i].points(j).y;
            PLfloat z = plAutomaticPlanner::_defectSiteGrids[i].points(j).z;
            plDraw::sphere( plVector3(x,y,z), 0.09f );
            
        }
        
        PLuint count = plAutomaticPlanner::stateGrafts[0];
        
        for (PLuint j=1; j<=count; j++)
        {
            PLfloat r = (j % 100) * 0.01;
            PLfloat g = (j % 1000) * 0.001;
            PLfloat b = (j % 20) * 0.05;
        
            plColourStack::load( r, g, b );
            plAutomaticPlanner::DEBUG_MESH[ plAutomaticPlanner::stateGrafts[j] ].draw();
        
        }
        /*
        for (PLuint j=0; j<plAutomaticPlanner::DEBUG_MESH.size(); j++)
        {
            PLfloat r = (j % 100) * 0.01;
            PLfloat g = (j % 1000) * 0.001;
            PLfloat b = (j % 20) * 0.05;
        
            plColourStack::load( r, g, b );
            plAutomaticPlanner::DEBUG_MESH[j].draw();
        }
        */
    }
    
    /* DEBUG FOR OCTREES
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
    PLfloat xmargin = (plWindow::width() / plWindow::height()- 1) / 2.0;
    
    plMatrix44 ortho( -1, 1, -1, 1, -1, 1);

    plMatrix44 camera( 1, 0,  0, 0,
                       0, 1,  0, 0,
                       0, 0, -1, 0,
                       0, 0,  0, 1 ); 
                       
    plShaderStack::push( _textureShader );                   
    plProjectionStack::push( ortho );   // ortho projection
    plModelStack::push( plMatrix44() ); // identity model matrix
    plCameraStack::push( camera );      // default camera matrix
    {
        glDisable( GL_DEPTH_TEST );

        _arthroTextureToDraw->draw();
         
        glEnable( GL_DEPTH_TEST ); 
    }       
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
        _graftEditorToDraw->drawHandles();
    }
    
    // draw editor menus
    if ( (_boundaryEditorToDraw != NULL || _graftEditorToDraw != NULL) && _planToDraw != NULL)
    {        
        plCameraStack::push( plMatrix44() );                                                        // identity viewing matrix   
        plProjectionStack::push( plMatrix44( 0, plWindow::width(), 0, plWindow::height(), -1, 1) ); // ortho, viewport dimensions
        {
            _boundaryEditorToDraw->drawMenu( *_planToDraw );
            _graftEditorToDraw->drawMenu( *_planToDraw );
        }
        plCameraStack::pop(); 
        plProjectionStack::pop();
    }   

}

