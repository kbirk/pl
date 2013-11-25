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
        _techniques[ PL_TRANSPARENCY_TECHNIQUE ] = std::shared_ptr<plTransparencyTechnique>( new plTransparencyTechnique() );
    }
     

}
