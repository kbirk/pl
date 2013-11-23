#include "plRenderer.h"


namespace plRenderer
{

    plRenderMap _renderMap;

    std::map< PLuint, std::shared_ptr<plRenderTechnique> > _techniques;

    // private function prototypes
    void _initTechniques();  
    
    
    void init()
    {
        _initTechniques();
    } 

    
    void queue( const plRenderable& renderable )
    {    
        // it would be nice to have each technique contain its own set of rc's
        renderable.extractRenderComponents( _renderMap );
    }
    
    
    void draw()
    {
        for ( auto& pair : _techniques )
        {
            // get technique ptr and enum
            PLuint techniqueEnum = pair.first;
            auto&  technique = pair.second;
            technique->render( _renderMap[ techniqueEnum ] );
        }

        // clear map for this frame
        _renderMap.clear();
    }
    

    void _initTechniques()
    {
        // create techniques
        _techniques[ PL_PLAN_TECHNIQUE ]        = std::shared_ptr<plPlanTechnique>( new plPlanTechnique() );
        _techniques[ PL_OUTLINE_TECHNIQUE ]     = std::shared_ptr<plOutlineTechnique>( new plOutlineTechnique() );
        _techniques[ PL_SCREEN_QUAD_TECHNIQUE ] = std::shared_ptr<plScreenQuadTechnique>( new plScreenQuadTechnique() );
        _techniques[ PL_ARTHRO_CAM_TECHNIQUE ]  = std::shared_ptr<plArthroCamTechnique>( new plArthroCamTechnique() );

        /*
        _techniques[ PL_ARTHRO_CAM_TECHNIQUE ] = std::shared_ptr<plPlannerTechnique>( new plArthoCamTechnique() );
        */
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
