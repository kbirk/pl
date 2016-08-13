#include "plGraft.h"

plGraft::plGraft()
    :   _recipient( PL_PICKING_INDEX_GRAFT_DEFECT ), _harvest( PL_PICKING_INDEX_GRAFT_DONOR )
{
    _generateCaps();
    snapMarkDirection();
}


plGraft::plGraft( const plPlug &harvest, const plPlug &recipient, PLfloat radius, PLfloat length, const plVector3 &markDirection  )
    :   _recipient( recipient ), _harvest( harvest ), _radius( radius ), _markDirection( markDirection ), _length( length )
{
    _generateCaps();
    snapMarkDirection();
}


void plGraft::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    // Draw at harvest location
    plModelStack::push( _harvest.finalTransform().matrix() );
    plPickingStack::loadBlue( PL_PICKING_INDEX_GRAFT_DONOR );
    _extractGraftRenderComponents( renderMap, technique );
    plModelStack::pop();

    // Draw at recipient location
    plModelStack::push( _recipient.finalTransform().matrix() );
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
        plRenderer::queueSphere( technique, _markPositions[0], 0.5f );
    }
    else
    {
        if ( _isSelected )
        {
            // draw cartilage cap
            _cartilageCap.extractRenderComponents( renderMap, PL_OUTLINE_TECHNIQUE );

            PLint id = 0;
            PLfloat SPACING = 7.0f;
            PLfloat LINE_WIDTH = 0.08f;
            PLfloat LINE_LENGTH = 5.0f;

            //plColourStack::load( plVector4( 1, 1, 1, 1 ) );
            plPickingStack::loadRed( PL_PICKING_TYPE_GRAFT_ARTHRO_PROJECTION );

            // i
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[0], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );

            // ii
            plModelStack::push();
            plPickingStack::loadBlue( id++ );
            plModelStack::rotate( -SPACING/2, plVector3( 0, 1, 0 ) );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[1], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[1], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::pop();

            // iii
            plModelStack::push();
            plPickingStack::loadBlue( id++ );
            plModelStack::rotate( -SPACING, plVector3( 0, 1, 0 ) );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[2], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::pop();

            // iiii
            plModelStack::push();
            plPickingStack::loadBlue( id++ );
            plModelStack::rotate( (-3/2)*SPACING, plVector3( 0, 1, 0 ) );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::rotate( SPACING, plVector3( 0, 1, 0 ) );
            plPickingStack::loadBlue( id++ );
            plRenderer::queueCylinder( PL_OUTLINE_TECHNIQUE, _markPositions[3], plVector3(0, 1, 0 ), LINE_WIDTH, LINE_LENGTH );
            plModelStack::pop();

        }
    }
}


void plGraft::_generateCaps()
{
    // generate cap polygons
    _cartilageCap.generateCap( ( const plOctreeMesh& )_harvest.mesh(), _harvest.finalTransform(), _radius );
    _boneCap.generateCap     ( ( const plOctreeMesh& )_harvest.mesh(), _harvest.finalTransform(), _radius );

    // generate vaos
    _cartilageCap.generateVAO( _radius, _length, _boneCap.perimeter );
    _boneCap.generateVAO( _radius, _length );
}


void plGraft::setMarkDirection( const plVector3& direction )
{
    // ensure direction is orthogonal
    _markDirection = plVector3( direction.x, 0.0f, direction.z ).normalize();
    _generateMarkPositions();
}


void plGraft::snapMarkDirection()
{
    // calculate seperation vector between grafts
    plVector3 up = plCameraStack::up();
    plVector3 screenNormal = -plCameraStack::direction();

    // ray cast up, this gets up vector overlayed onto plane (projected from screen direction, rather than projected by surface normal )
    plIntersection harvestIntersection = plMath::rayIntersect( _harvest.surfaceTransform().origin() + plCameraStack::up(),
                                                               plCameraStack::direction(),
                                                               _harvest.surfaceTransform().origin(),
                                                               _harvest.surfaceTransform().y()  );

    // find the up direction overlayed on plane, not projected
    plVector3 upHarvestProj = ( harvestIntersection.point - _harvest.surfaceTransform().origin() ).normalize();

    // ray cast up, this gets up vector overlayed onto plane (projected from screen direction, rather than projected by surface normal )
    plIntersection recipientIntersection = plMath::rayIntersect( _recipient.surfaceTransform().origin() + plCameraStack::up(),
                                                                 plCameraStack::direction(),
                                                                 _recipient.surfaceTransform().origin(),
                                                                 _recipient.surfaceTransform().y()  );
    // get vector in graft local space
    plVector3 graftZ = _harvest.finalTransform().applyNormalInverse( upHarvestProj );

    // calulate this direction for recipient graft in world space
    plVector3 recipientDirection = ( _recipient.finalTransform().applyNormal( graftZ ) ).normalize();

    // project up onto recipient
    plVector3 upRecipientProj = ( recipientIntersection.point - _recipient.surfaceTransform().origin() ).normalize();

    // find angle between up proj and marker direction
    PLfloat recipientAngle = ( upRecipientProj ).signedAngle( recipientDirection, _recipient.surfaceTransform().y() );

    plMatrix44 rotation;    rotation.setRotation( -recipientAngle/2.0f , plVector3( 0, 1, 0 ) );

    setMarkDirection( rotation * graftZ );

    /*
    _markDirection = rotation * graftZ; //_harvest.finalTransform().applyNormalInverse( rotation * upHarvestProj ).normalize(); // perfect casted up
    _generateMarkPositions();
    */
}


void plGraft::_generateMarkPositions()
{
    //plMatrix44 offsetRot;   offsetRot.setRotation( _markAngleOffset, plVector3( 0, 1, 0 ) );

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
            break;

        default:

            std::cerr << "plGraft move() error: invalid type enumeration provided" << std::endl;
            break;
    }

    // update the marker position
    _generateMarkPositions();
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
            break;

        default:

            std::cerr << "plGraft rotate() error: invalid type enumeration provided" << std::endl;
            break;
    }

    // update the marker position
    _generateMarkPositions();
}


void plGraft::rotate( PLuint type, PLfloat angleDegrees )
{
        switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:

            _harvest.rotate( angleDegrees );
            _generateCaps();
            break;

        case PL_PICKING_INDEX_GRAFT_DEFECT:

            _recipient.rotate( angleDegrees );
            break;

        default:

            std::cerr << "plGraft rotate() error: invalid type enumeration provided" << std::endl;
            break;
    }

    // update the marker position
    _generateMarkPositions();
}
