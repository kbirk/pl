#include "plGraft.h"

plGraft::plGraft()
    :   _recipient( PL_PICKING_INDEX_GRAFT_DEFECT ), _harvest( PL_PICKING_INDEX_GRAFT_DONOR )
{
}


plGraft::plGraft( const plPlug &harvest, const plPlug &recipient, PLfloat radius, PLfloat length, const plVector3 &markDirection  )
    :   _recipient( recipient ), _harvest( harvest ), _radius( radius ), _markDirection( markDirection ), _length( length ), _markAngleOffset( 0 )
{
    _generateCaps();   
}


void plGraft::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    // Draw at harvest location
    plModelStack::push( _harvest.matrix() );
    plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );                  
    _extractGraftRenderComponents( renderMap, technique );
    plModelStack::pop();

    // Draw at recipient location
    plModelStack::push( _recipient.matrix() );
    plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DEFECT );
    _extractGraftRenderComponents( renderMap, technique );
    plModelStack::pop();
    
    if ( !_inArthroView )
    {
         // draw axis
        _harvest.transform().extractRenderComponents( renderMap, technique );
        _recipient.transform().extractRenderComponents( renderMap, technique );
        
        plVector3 direction = ( plCameraStack::position() - _harvest.transform().origin() ).normalize();

    }
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
        plRenderer::queueSphere( technique, _markPositions[0], 0.5f );
    }
    else
    {
        if ( _isSelected )
        {
            // draw cartilage cap
            _cartilageCap.extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );

            PLint id = 101;
            PLfloat SPACING = 7.0f;
            PLfloat LINE_WIDTH = 0.08f;
            PLfloat LINE_LENGTH = 1.0f;
              
            //plColourStack::load( plVector4( 1, 1, 1, 1 ) ); 
  
            // i 
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[0], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            
            // ii
            plModelStack::push();
            plPickingStack::loadRed( id++ );
            plModelStack::rotate( -SPACING/2, plVector3( 0, 1, 0 ) );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[1], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[1], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::pop();
            
            // iii
            plModelStack::push();
            plPickingStack::loadRed( id++ );
            plModelStack::rotate( -SPACING, plVector3( 0, 1, 0 ) );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::pop();
            
            // iiii
            plModelStack::push();
            plPickingStack::loadRed( id++ );
            plModelStack::rotate( (-3/2)*SPACING, plVector3( 0, 1, 0 ) );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadRed( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::pop();

        }
    }
}


void plGraft::_generateCaps()
{
    // generate cap polygons
    _cartilageCap.generateCap( ( const plOctreeMesh& )_harvest.mesh(), _harvest.finalTransform(), _radius );
    _boneCap.generateCap( ( const plOctreeMesh& )_harvest.mesh(), _harvest.finalTransform(), _radius );

    // generate vaos 
    _cartilageCap.generateVAO( _radius, _length, _boneCap.perimeter );
    _boneCap.generateVAO( _radius, _length );

    // update values
    _updateMarker();      
}


void plGraft::_updateMarker()
{
    _updateMarkDirection();
    _generateMarkPositions();
}


void plGraft::setMarkOffset( PLfloat offset )
{
    _markAngleOffset = offset;
    _updateMarker();  
}


void plGraft::_updateMarkDirection()
{
    // calculate seperation vector between grafts
    plVector3 harvestToRecipient = ( _recipient.transform().origin() - _harvest.transform().origin() ).normalize();

    // ensure this vector is othrogonal with graft transform
    plVector3 x = ( _harvest.transform().y() ^ harvestToRecipient ).normalize();
    plVector3 z = ( x ^ _harvest.transform().y() ).normalize();
    
    // get vector in graft local space
    plVector3 graftZ = _harvest.transform().applyNormalInverse( z );  
            
    // calulate this direction for recipient graft in world space
    plVector3 recipientDirection = ( _recipient.transform().applyNormal( graftZ ) ).normalize();        

    //plRenderer::queueArrow( PL_PLAN_TECHNIQUE, graft->harvest().transform().origin(), harvestToRecipient, ( graft->recipient().transform().origin() - graft->harvest().transform().origin() ).length() );

    // calc how far this angle is offset
    PLfloat recipientAngle = ( -harvestToRecipient ).signedAngle( recipientDirection, _recipient.transform().y() );

    // rotate marker by half the offset
    plMatrix44 rot;   rot.setRotation( -recipientAngle / 2.0f , plVector3( 0, 1, 0 ) );
            
    _markDirection = ( rot * _harvest.transform().applyNormalInverse( z ) ).normalize();
}


void plGraft::_generateMarkPositions()
{
    plMatrix44 offsetRot;   offsetRot.setRotation( _markAngleOffset, plVector3( 0, 1, 0 ) );
     
    for ( PLuint i=0; i < 4; i++ )
    {
        plMatrix44 rotation;  rotation.setRotationD( i*-90.0f,  plVector3( 0, 1, 0 ) );
    
        // Mark at tool alignment direction on cartilage
        _markPositions[i] = _radius * ( rotation * offsetRot * _markDirection ).normalize();

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


plTransform plGraft::transform( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest.finalTransform();        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient.finalTransform();
            
        default:
        
            std::cerr << "plGraft transform()() error: invalid type enumeration provided, defaulting to recipient" << std::endl;
            return _recipient.finalTransform();   
    } 
}


const plVector3& plGraft::surfaceNormal( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest.transform().y();        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient.transform().y();
            
        default:
        
            std::cerr << "plGraft transform()() error: invalid type enumeration provided, defaulting to recipient" << std::endl;
            return _recipient.transform().y();   
    } 

}


const plPlug &plGraft::plug( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:      return _harvest;        
        case PL_PICKING_INDEX_GRAFT_DEFECT:     return _recipient;
            
        default:
        
            std::cerr << "plGraft plug() error: invalid type enumeration provided, defaulting to recipient" << std::endl;
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
            _updateMarker();
            break;
            
        default:
        
            std::cerr << "plGraft move() error: invalid type enumeration provided" << std::endl;
            break;    
    } 
}

void plGraft::rotate( PLuint type, const plVector3& y )
{
        switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            _harvest.rotate( y );            
            _generateCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            _recipient.rotate( y );
            _updateMarker();
            break;
            
        default:
        
            std::cerr << "plGraft rotate() error: invalid type enumeration provided" << std::endl;
            break;    
    } 
}


