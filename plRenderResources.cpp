#include "plRenderResources.h"


namespace plRenderResources
{

    std::map< PLuint, std::shared_ptr<plFBO> >    _fbos;
    std::map< PLuint, std::shared_ptr<plShader> > _shaders;

    // private function forward declarations
    void _initFBOs( PLuint width, PLuint height );
    void _initShaders();
    
    
    const std::shared_ptr< plFBO >& fbos( PLuint id )
    {
        return _fbos[ id ];
    }
    
    
    const std::shared_ptr< plShader >& shaders( PLuint id )
    {
        return _shaders[ id ];
    }


    void init()
    {
        _initFBOs( 1, 1 );
        _initShaders();
    } 
    
    
    void reshape( PLuint width, PLuint height )
    {      
        _initFBOs( width, height );
    }


    void _initFBOs( PLuint width, PLuint height )
    {
        _fbos.clear();
        
        // create main fbo
        _fbos[ PL_MAIN_FBO ] = std::shared_ptr<plFBO>( new plFBO() );
        
        // main render buffer
        _fbos[ PL_MAIN_FBO ]->attach( GL_COLOR_ATTACHMENT0, 
                                     std::shared_ptr<plTexture2D>( new plTexture2D( width, height, GL_RGBA8,  GL_RGBA, GL_UNSIGNED_BYTE, NULL ) ) );
        // texture for outlining
        _fbos[ PL_MAIN_FBO ]->attach( GL_COLOR_ATTACHMENT1,     
                                     std::shared_ptr<plTexture2D>( new plTexture2D( width, height, GL_RGBA32I,  GL_RGBA_INTEGER, GL_INT, NULL ) ) );
        // **CURRENTLY UNUSED**
        _fbos[ PL_MAIN_FBO ]->attach( GL_COLOR_ATTACHMENT2,
                                     std::shared_ptr<plTexture2D>( new plTexture2D( width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL ) ) );
        // arthro camera texture
        _fbos[ PL_MAIN_FBO ]->attach( GL_COLOR_ATTACHMENT3,
                                     std::shared_ptr<plTexture2D>( new plTexture2D( width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL ) ) );
        // colour picking texture
        _fbos[ PL_MAIN_FBO ]->attach( GL_COLOR_ATTACHMENT4,
                                     std::shared_ptr<plTexture2D>( new plTexture2D( width, height, GL_RGB32I, GL_RGB_INTEGER, GL_INT, NULL ) ) );
        // depth-stencil buffer texture
        _fbos[ PL_MAIN_FBO ]->attach( GL_DEPTH_ATTACHMENT, 
                                      GL_STENCIL_ATTACHMENT, 
                                      std::shared_ptr<plTexture2D>( new plTexture2D( width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL ) ) );
    }
    
    
    void _initShaders()
    {
        // create shader objects
        _shaders[ PL_MINIMAL_SHADER      ] = std::shared_ptr<plVertexFragmentShader>( new plVertexFragmentShader( PL_FILE_PREPATH"shaders/minimal.vert", PL_FILE_PREPATH"shaders/minimal.frag" ) );  
        _shaders[ PL_PHONG_SHADER        ] = std::shared_ptr<plVertexFragmentShader>( new plVertexFragmentShader( PL_FILE_PREPATH"shaders/phong.vert",   PL_FILE_PREPATH"shaders/phong.frag"   ) ); 
        _shaders[ PL_OUTLINE_SHADER      ] = std::shared_ptr<plVertexFragmentShader>( new plVertexFragmentShader( PL_FILE_PREPATH"shaders/outline.vert", PL_FILE_PREPATH"shaders/outline.frag" ) );  
        _shaders[ PL_TEXTURE_2D_SHADER   ] = std::shared_ptr<plVertexFragmentShader>( new plVertexFragmentShader( PL_FILE_PREPATH"shaders/texture.vert", PL_FILE_PREPATH"shaders/texture.frag" ) );  
        _shaders[ PL_ARTHRO_CAM_SHADER   ] = std::shared_ptr<plVertexFragmentShader>( new plVertexFragmentShader( PL_FILE_PREPATH"shaders/arthro.vert",  PL_FILE_PREPATH"shaders/arthro.frag"  ) );
        _shaders[ PL_FBO_SHADER          ] = std::shared_ptr<plVertexFragmentShader>( new plVertexFragmentShader( PL_FILE_PREPATH"shaders/fbo.vert",     PL_FILE_PREPATH"shaders/fbo.frag"     ) );
    }
    
}

