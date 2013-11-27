#include "plGraft.h"

plGraft::plGraft()
    :   _recipient( PL_PICKING_INDEX_GRAFT_DEFECT ), _harvest( PL_PICKING_INDEX_GRAFT_DONOR )
{
}


plGraft::plGraft( const plPlug &harvest, const plPlug &recipient, PLfloat radius, PLfloat length, const plVector3 &markDirection  )
    :   _recipient( recipient ), _harvest( harvest ), _radius( radius ), _markDirection( markDirection ), _length( length )
{
    _generateCaps();   
}


void plGraft::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    // Draw at harvest location
    plModelStack::push( _harvest.transform().matrix() );
    plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );                  
    _extractGraftRenderComponents( renderMap, technique );
    plModelStack::pop();

    // Draw at recipient location
    plModelStack::push( _recipient.transform().matrix() );
    plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DEFECT );
    _extractGraftRenderComponents( renderMap, technique );
    plModelStack::pop();
}


void plGraft::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


void plGraft::_extractGraftRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT ); 

    if ( !_inArthroView )
    {
        // draw cartilage cap
        _cartilageCap.extractRenderComponents( renderMap, technique );
        // draw bone cap
        _boneCap.extractRenderComponents( renderMap, technique );
        
        // draw marker   
        plColourStack::load( PL_GRAFT_MARKER_COLOUR );
        plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_MARKER );
        plRenderer::queue( plSphere( technique, _markPositions[0], 0.5f ) );
    }
    else
    {
        if ( _isSelected )
        {
            // draw cartilage cap
            _cartilageCap.extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );

            PLint id = 101;
            PLfloat SPACING = 5.0f;
            PLfloat LINE_WIDTH = 0.04f;
            PLfloat LINE_LENGTH = 10.0f;
              
            // i 
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[0], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            
            // ii
            plModelStack::push();
            plPickingStack::loadRed( id++ );
            plModelStack::rotate( -SPACING/2, plVector3( 0, 1, 0 ) );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[1], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[1], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::pop();
            
            // iii
            plModelStack::push();
            plPickingStack::loadRed( id++ );
            plModelStack::rotate( -SPACING, plVector3( 0, 1, 0 ) );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::pop();
            
            // iiii
            plModelStack::push();
            plPickingStack::loadRed( id++ );
            plModelStack::rotate( (-3/2)*SPACING, plVector3( 0, 1, 0 ) );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queue( plCone( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, 0, LINE_LENGTH ) );
            plModelStack::pop();

        }
    }
}


void plGraft::_generateCaps()
{
    // generate cap polygons
    _cartilageCap.generateCap( ( const plOctreeMesh& )_harvest.mesh(), _harvest.transform(), _radius );
    _boneCap.generateCap(  ( const plOctreeMesh& )_harvest.mesh(), _harvest.transform(), _radius );

    // generate vaos 
    _cartilageCap.generateVAO( _radius, _length, _boneCap.perimeter );
    _boneCap.generateVAO( _radius, _length );

    // update values
    _updateMarkPosition();      
}


void plGraft::_updateMarkPosition()
{
    for ( PLuint i=0; i < 4; i++ )
    {
        plMatrix44 rotation;  rotation.setRotationD( i*-90.0f,  plVector3( 0, 1, 0 ) );
    
        // Mark at tool alignment direction on cartilage
        _markPositions[i] = _radius * ( rotation * _markDirection ).normalize();

        // First, find the closest top perimeter point in the mark direction.
        float minDist = FLT_MAX;
        float minY;

        const std::vector<plPointAndAngle>& perimeter = ( _cartilageCap.triangles.empty() ) ? _boneCap.perimeter : _cartilageCap.perimeter;

        for ( PLuint j=0; j<perimeter.size(); j++ ) 
        {
            const plVector3 &v = perimeter[j].point;
            float dist = (v.x-_markPositions[i].x)*(v.x-_markPositions[i].x) + (v.z-_markPositions[i].z)*(v.z-_markPositions[i].z);
            if (dist < minDist) 
            {
                minDist = dist;
                minY = v.y;
            }
        }

        // Draw marker  
        _markPositions[i].y = minY;
    }
}


void plGraft::setMark( const plVector3 &direction )
{   
    _markDirection = direction.normalize();    
    _updateMarkPosition();
}


const plTransform& plGraft::transform( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest.transform();        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient.transform();
            
        default:
        
            std::cerr << "plGraft transform()() error: invalid type enumeration provided, defaulting to recipient \n";
            return _recipient.transform();   
    } 
}


const plVector3& plGraft::surfaceNormal( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest.surfaceNormal();        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient.surfaceNormal();
            
        default:
        
            std::cerr << "plGraft transform()() error: invalid type enumeration provided, defaulting to recipient \n";
            return _recipient.surfaceNormal();   
    } 

}


const plPlug &plGraft::plug( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest;        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient;
            
        default:
        
            std::cerr << "plGraft plug() error: invalid type enumeration provided, defaulting to recipient \n";
            return _recipient;   
    } 
}


void plGraft::move( PLuint type, const plVector3& origin, const plVector3& y )
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            _harvest.move( origin, y );            
            _generateCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.move( origin, y );
            break;
            
        default:
        
            std::cerr << "plGraft translate() error: invalid type enumeration provided \n";
            break;    
    } 
}


