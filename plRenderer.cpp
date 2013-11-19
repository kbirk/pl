#include "plRenderer.h"

namespace plRenderer
{
    plFBO *_fbo;
    
    std::shared_ptr< plTexture2D > _opaqueTexture;
    std::shared_ptr< plTexture2D > _outlineTexture;
    std::shared_ptr< plTexture2D > _pickingTexture;
    std::shared_ptr< plTexture2D > _depthStencilTexture;
                  
    std::map< PLuint, // technique enum
              std::map< PLuint, // render component enum
                        std::vector< plRenderComponent > > _renderLists;
    
    plPickingInfo _previousPick;
    PLuint _viewportHeight;
    PLuint _viewportWidth;
    PLuint _vBuffer, _hBuffer;

    
    // private function prototypes
    void _setOpenGLState();
    void _beginDrawing();
    void _endDrawing();
    void _updateViewport( PLuint width, PLuint height );
    void _generateFBO( PLuint width, PLuint height );


    void init()
    {
        _setOpenGLState();
        _generateFBO( 1, 1 );
        plProjectionStack::load( plProjection( PL_FIELD_OF_VIEW , PL_ASPECT_RATIO, PL_NEAR_PLANE, PL_FAR_PLANE ) ); 
    } 
    
    
    void _updateViewport( PLuint width, PLuint height )
    {
        _viewportHeight = width / PL_ASPECT_RATIO ;

        if ( _viewportHeight <= height )
        {
            _vBuffer = ( height - _viewportHeight )*0.5f;
            _hBuffer = 0;
            _viewportWidth = width;
        }
        else
        {
            _viewportWidth = height * PL_ASPECT_RATIO;
            _viewportHeight = height;
            _hBuffer = ( width - _viewportWidth )*0.5f;
            _vBuffer = 0;
        }    
    }
    
    
    void _generateFBO( PLuint width, PLuint height )
    {
        delete _fbo;
        _fbo = new plFBO();
        
        _opaqueTexture       = std::shared_ptr<plTexture2D> ( new plTexture2D( width, height, GL_RGBA8,  GL_RGBA,        GL_UNSIGNED_BYTE, NULL ) );
        _outlineTexture      = std::shared_ptr<plTexture2D> ( new plTexture2D( width, height, GL_RGBA8,  GL_RGBA,        GL_UNSIGNED_BYTE, NULL ) );
        _pickingTexture      = std::shared_ptr<plTexture2D> ( new plTexture2D( width, height, GL_RGB32I, GL_RGB_INTEGER, GL_INT,           NULL  ) );        
        _depthStencilTexture = std::shared_ptr<plTexture2D> ( new plTexture2D( width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL ) );

        _fbo->attach( _opaqueTexture,       GL_COLOR_ATTACHMENT0  );
        _fbo->attach( _outlineTexture,      GL_COLOR_ATTACHMENT1  );
        _fbo->attach( _pickingTexture,      GL_COLOR_ATTACHMENT2  );
        _fbo->attach( _depthStencilTexture, GL_DEPTH_ATTACHMENT   );
        _fbo->attach( _depthStencilTexture, GL_STENCIL_ATTACHMENT );  
    }
    
    
    void resize( PLuint width, PLuint height )
    {      
        _updateViewport( width, height );
        _generateFBO( _viewportWidth, _viewportHeight );        
    }
    
    
    void queue( const plRenderable& renderable )
    {
        renderable.extractRenderComponents( _renderQueue );

        
        
    }


    

    void _testTexture()
    {
        // VAO GENERATION
        std::vector<plVector3> vertices;
        std::vector<PLuint>    indices;
        std::vector<PLuint>    attributeTypes;
        
        attributeTypes.push_back( PL_POSITION_ATTRIBUTE );  
        attributeTypes.push_back( PL_TEXCOORD_ATTRIBUTE );
        
        // position                                     // texture coord       
        vertices.push_back( plVector3( -1, -1, 0 ) );   vertices.push_back( plVector3( 0,0,0) );   
        vertices.push_back( plVector3(  1, -1, 0 ) );   vertices.push_back( plVector3( 1,0,0) );   
        vertices.push_back( plVector3(  1,  1, 0 ) );   vertices.push_back( plVector3( 1,1,0) );    
        vertices.push_back( plVector3( -1,  1, 0 ) );   vertices.push_back( plVector3( 0,1,0) );
        
        indices.push_back( 0 );   indices.push_back( 1 );   indices.push_back( 2 );
        indices.push_back( 0 );   indices.push_back( 2 );   indices.push_back( 3 );
           
        static plVAO vao( vertices, attributeTypes, indices );
        ///
        
        // TEX
        std::vector<PLuint> texUnits;
        texUnits.push_back( 0 );
        texUnits.push_back( 1 );
        //
              
        plMatrix44 ortho( -1, 1, -1, 1, -1, 1 );

        plMatrix44 camera( 1, 0,  0, 0,
                           0, 1,  0, 0,
                           0, 0, -1, 0,
                           0, 0,  0, 1 ); 
            
        plTexture2DStack::push ( *_opaqueTexture,  0 );    
        plTexture2DStack::push ( *_outlineTexture, 1 );       
        plShaderStack::push    ( PL_FBO_SHADER );                   
        plProjectionStack::push( ortho );        // ortho projection
        plModelStack::push     ( plMatrix44() ); // identity model matrix
        plCameraStack::push    ( camera );       // default camera matrix

        _renderQueue.insert( plRenderComponent( &vao, texUnits ) );
      
        plProjectionStack::pop(); 
        plModelStack::pop();       
        plCameraStack::pop();    
        plShaderStack::pop();
        plTexture2DStack::pop( 0 );
        plTexture2DStack::pop( 1 );
    }


    void draw()
    {
        _beginDrawing();
        _fbo->bind();   
        
        // clear opaque buffer
        /*
        GLenum buffers0[2] = { GL_COLOR_ATTACHMENT0, GL_NONE };  
        _fbo->setDrawBuffers( buffers0, 2 );
        glClearColor( PL_CLEAR_COLOUR );          
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        
        // clear blend buffer
        GLenum buffers1[2] = { GL_NONE, GL_COLOR_ATTACHMENT1 };  
        _fbo->setDrawBuffers( buffers1, 2 );        
        */
                
        // clear fbo
        glClearColor( 0, 0, 0, 0 );          
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        glViewport( 0, 0, _viewportWidth, _viewportHeight ); 
        //
          
          
             
        for ( const std::set renderQueue : _renderQueues )
        {
            for ( const plRenderComponent& renderComponent : _renderQueue )
            { 
                renderComponent.draw();   
            }
        }     
             
        
        _endDrawing();
        
        // this should not be manual, should be enabled by flags  
        _fbo->unbind();  
        glClearColor( PL_CLEAR_COLOUR );          
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );         
        glViewport( _hBuffer, _vBuffer, _viewportWidth, _viewportHeight );       
        //
        
        _renderQueue.clear();  
        _testTexture();
        for ( const plRenderComponent& renderComponent : _renderQueue )
        { 
            renderComponent.draw();   
        }
        
        _renderQueue.clear();
    }
    

    void _setOpenGLState()
    {
        glEnable( GL_CULL_FACE );
        glCullFace( GL_BACK );    
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable( GL_DEPTH_TEST ); 
        glEnable( GL_BLEND );  
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  
        glDepthFunc( GL_LEQUAL );
    }

    
    //template< typename T>
    //plPixel<T>        
    const plPickingInfo& pickPixel( PLuint x, PLuint y )
    {
        plPixel<PLint> pick = _fbo->readPixel<PLint>( GL_COLOR_ATTACHMENT2, x - _hBuffer, y - _vBuffer );

        _previousPick = plPickingInfo( pick.r, pick.g, pick.b );

        std::cout << "picking: " << _previousPick.r << " " << _previousPick.g << " " << _previousPick.b << "\n"; 
    
        return _previousPick;   
    }


    //template< typename T>
    //plPixel<T> 
    const plPickingInfo& previousPick()
    {
        return _previousPick;
    }

    /*
    void draw( PLuint x, PLuint y )
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
            _drawScene( x, y );
        }

        _endDrawing();

        // clear queues from this frame
        _clearRenderQueue();
    }


    void _beginPicking()
    {
        glDisable( GL_BLEND );

        // bind picking shader
        plShaderStack::push( _pickingShader );
        
        // bind picking texture
        plPicking::bind();

        // clear picking texture
        glClearColor(0,0,0,0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

    }


    void _endPicking()
    {
        plShaderStack::pop();
        plPicking::unbind();  
    }
    */

    void _beginDrawing()
    {
        glEnable( GL_BLEND );
        
        glClearColor( PL_CLEAR_COLOUR );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        
        //glStencilFunc( GL_ALWAYS, 0x00, 0x00);               // only render to bits = 0 (have not been written)
	    //plShaderStack::push( _phongShader );
    }


    void _endDrawing()
    {
       // plShaderStack::pop(); 
    }

    /*
    void _drawScene( PLuint x, PLuint y )
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
            plCameraStack::push( plMatrix44() );                                // identity viewing matrix
            plProjectionStack::push( plMatrix44( 0, plWindow::viewportWidth(),  // ortho, viewport dimensions
                                                 0, plWindow::viewportHeight(), 
                                                 -1, 1) ); 
            {
                _boundaryEditorToDraw->drawMenu( *_planToDraw, x, y );
                _graftEditorToDraw->drawMenu   ( *_planToDraw, x, y );
            }
            plProjectionStack::pop(); 
            plCameraStack::pop();                
            plShaderStack::pop();   
        }   
        
        // draw probe
        if (_probeToDraw != NULL)
        {
            _probeToDraw->draw();
        }
        
        // draw arthroscope
        if (_scopeToDraw != NULL)
        {
            _scopeToDraw->draw();
        }
        
        if (_chessBoardToDraw != NULL)
        {
            _chessBoardToDraw->draw();
        }
        
        /* DEBUG FOR GRID POINTS
        for (PLuint i=0; i<plAutomaticPlanner::_donorSiteGrids.size(); i++)
        {
            plColourStack::load( 0.9, 0.6, 0.2 );
            for (PLuint j=0; j<plAutomaticPlanner::_donorSiteGrids[i].gridSize(); j++)
            {
                PLfloat x = plAutomaticPlanner::_donorSiteGrids[i].points(j).x;
                PLfloat y = plAutomaticPlanner::_donorSiteGrids[i].points(j).y;
                PLfloat z = plAutomaticPlanner::_donorSiteGrids[i].points(j).z;
                
                plDraw::sphere( plVector3(x,y,z), 0.09f );
            }
        }
        for (PLuint i=0; i<plAutomaticPlanner::_defectSiteGrids.size(); i++)
        {    
            plColourStack::load( 0.2, 0.6, 0.9 );
            
            for (PLuint j=0; j<plAutomaticPlanner::_defectSiteGrids[i].gridSize(); j++)
            {
                PLfloat x = plAutomaticPlanner::_defectSiteGrids[i].points(j).x;
                PLfloat y = plAutomaticPlanner::_defectSiteGrids[i].points(j).y;
                PLfloat z = plAutomaticPlanner::_defectSiteGrids[i].points(j).z;
                plDraw::sphere( plVector3(x,y,z), 0.09f );
                
            }
        }
        */
        
        /*
        // draw scanner information
        if (_scanToDraw != NULL)
        {
            _scanToDraw->draw();
        }

        // draw debug spheres
        for (PLuint i = 0; i < _debugSpheresToDraw.size(); i++)
        {
            _debugSpheresToDraw[i].draw();

        }

        // draw debug transforms
        for (PLuint i = 0; i < _debugTransformsToDraw.size(); i++)
        {
            _debugTransformsToDraw[i].draw();

        }

        // draw lines (currently just thin cylinders)

        for (PLuint i = 0; i < _laserLinesToDraw.size(); i++)
        {
            plColourStack::push( _laserLinesToDraw[i].colour );
            plDraw::laserLine( _laserLinesToDraw[i].origin,
                              _laserLinesToDraw[i].direction,
                              _laserLinesToDraw[i].length );
            plColourStack::pop();
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
            _planToDraw->models(i).cartilage.mesh().octree().draw();
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        */
        /*
    }


    void _drawArthroTexture()
    {
        plMatrix44 ortho( -1, 1, -1, 1, -1, 1 );

        plMatrix44 camera( 1, 0,  0, 0,
                           0, 1,  0, 0,
                           0, 0, -1, 0,
                           0, 0,  0, 1 ); 
                  
        plShaderStack::push    ( _textureShader );                   
        plProjectionStack::push( ortho );        // ortho projection
        plModelStack::push     ( plMatrix44() ); // identity model matrix
        plCameraStack::push    ( camera );       // default camera matrix
        {
            glDisable( GL_DEPTH_TEST );

            //_arthroTextureToDraw->draw();
             
            glEnable( GL_DEPTH_TEST ); 
        }       
        plProjectionStack::pop(); 
        plModelStack::pop();       
        plCameraStack::pop();    
        plShaderStack::pop();
    }


    void _drawScenePicking()
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
            plCameraStack::push( plMatrix44() );                                // identity viewing matrix
            plProjectionStack::push( plMatrix44( 0, plWindow::viewportWidth(),  // ortho, viewport dimensions
                                                 0, plWindow::viewportHeight(), 
                                                 -1, 1) ); 
            {
                _boundaryEditorToDraw->drawMenu( *_planToDraw, 0, 0 );
                _graftEditorToDraw->drawMenu   ( *_planToDraw, 0, 0 );
            }
            plCameraStack::pop(); 
            plProjectionStack::pop();
        }   

    }
    */
}
