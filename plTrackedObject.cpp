#include "plTrackedObject.h"

plTrackedObject::plTrackedObject()
{
}


plTrackedObject::plTrackedObject( const plDRBTransform &ToTrackedPoint,
                                  const plDRBTransform &ToTrackedEnd,
                                  const plDRBTransform &FemurDRBToFemurSTL, bool isArthro) {
    _DRBToTrackedPoint  = ToTrackedPoint.clone();
    _DRBToTrackedEnd    = ToTrackedEnd.clone();
    _FemurDRBtoSTL         = FemurDRBToFemurSTL.clone();
    _trackedTip         = plVector3(0, 0, 0);
    _trackedEnd         = plVector3(0, 0, 0);
    _rotationAxis       = plVector3(0, 0, 0);
    _tipWorldCoords     = plVector3(0, 0, 0);
    _endWorldCoords     = plVector3(0, 0, 0);
    _xAxis              = plVector3(1, 0, 0);
    _yAxis              = plVector3(0, 1, 0);
    _zAxis              = plVector3(0, 0, 1);
    _isVisible          = false;
    _isArthroscope      = isArthro;
}


void plTrackedObject::updatePosition( const plDRBTransform &DRBToWorld, const plDRBTransform &FemurToWorld) 
{
    _tipWorldCoords = _DRBToTrackedPoint.applyTransform( plVector3(0, 0, 0) );
    _tipWorldCoords = DRBToWorld.applyTransform(_tipWorldCoords);
    _trackedTip     = FemurToWorld.applyInverseTransform(_tipWorldCoords);
    _trackedTip     = _FemurDRBtoSTL.applyTransform(_trackedTip);

    _xAxis          = _DRBToTrackedPoint.applyTransform(plVector3(1, 0, 0));
    _xAxis          = DRBToWorld.applyTransform(_xAxis);
    _xAxis          = FemurToWorld.applyInverseTransform(_xAxis);
    _xAxis          = _FemurDRBtoSTL.applyTransform(_xAxis);
    _xAxis          = (_xAxis - _trackedTip).normalize();


    _yAxis          = _DRBToTrackedPoint.applyTransform(plVector3(0, 1, 0));
    _yAxis          = DRBToWorld.applyTransform(_yAxis);
    _yAxis          = FemurToWorld.applyInverseTransform(_yAxis);
    _yAxis          = _FemurDRBtoSTL.applyTransform(_yAxis);
    _yAxis          = (_yAxis - _trackedTip).normalize();

    _zAxis          = _DRBToTrackedPoint.applyTransform(plVector3(0, 0, 1));
    _zAxis          = DRBToWorld.applyTransform(_zAxis);
    _zAxis          = FemurToWorld.applyInverseTransform(_zAxis);
    _zAxis          = _FemurDRBtoSTL.applyTransform(_zAxis);
    _zAxis          = (_zAxis - _trackedTip).normalize();

    if (!_isArthroscope) 
    {
        _endWorldCoords = _DRBToTrackedEnd.applyTransform( plVector3(0, 0, 0) );
    }
    else 
    {
        // arthroscope end is rotated by 30 degrees from camera view direction
        // and must be handled differently
        _endWorldCoords = _DRBToTrackedEnd.applyTransform(plVector3(0, 0, -10));
    }
    _endWorldCoords = DRBToWorld.applyTransform(_endWorldCoords);
    _trackedEnd     = FemurToWorld.applyInverseTransform( _endWorldCoords);
    _trackedEnd     = _FemurDRBtoSTL.applyTransform(_trackedEnd);

    _rotationAxis   = ( ((_trackedEnd-_trackedTip).normalize()) ^ plVector3(0,0,1) ).normalize();
    _rotationAngle  = - acos((_trackedEnd-_trackedTip).normalize() * plVector3(0,0,1)) * 57.295779513082320876798154;
}


void plTrackedObject::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    // Draw the TA002 probe
    if ( _isVisible )
        plColourStack::load(0.6, 0.6, 0.6);
    else
        plColourStack::load(1.0, 0.3, 0.1);

    plModelStack::push();
    plModelStack::translate( _trackedTip );
    plModelStack::rotate( _rotationAngle, _rotationAxis );
    plRenderer::queueCone( technique, plVector3( 0, 0, 0 ), plVector3( 0, 0, 1 ), 1.0f, 0, 4.0f );
    plRenderer::queueCone( technique, plVector3( 0, 0, 4 ), plVector3( 0, 0, 1 ), 4.0f, 1.0f, 124.0f );
    plModelStack::pop();
}


void plTrackedObject::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


