#include "plTrackedObject.h"

plTrackedObject::plTrackedObject()
{
}

plTrackedObject::plTrackedObject( const plDRBTransform &ToTrackedPoint,
                                  const plDRBTransform &ToTrackedEnd,
                                  const plDRBTransform &FemurDRBToFemurSTL, bool isArthro) {
    _DRBToTrackedPoint  = ToTrackedPoint.clone();
    _DRBToTrackedEnd    = ToTrackedEnd.clone();
    _toFemurSTL         = FemurDRBToFemurSTL.clone();
    _trackedTip         = plVector3(0, 0, 0);
    _trackedEnd         = plVector3(0, 0, 0);
    _rotationAxis       = plVector3(0, 0, 0);
    _tipWorldCoords     = plVector3(0, 0, 0);
    _endWorldCoords     = plVector3(0, 0, 0);
    _zeroVec            = plVector3(0, 0, 0);
    _xAxis              = plVector3(1, 0, 0);
    _yAxis              = plVector3(0, 1, 0);
    _zAxis              = plVector3(0, 0, 1);
    _isVisible          = false;
    _isArthroscope      = isArthro;
}

void plTrackedObject::updatePosition( const plDRBTransform &DRBToWorld, const plDRBTransform &FemurToWorld) 
{
    _tipWorldCoords = _DRBToTrackedPoint.applyTransform(_zeroVec);
    _tipWorldCoords = DRBToWorld.applyTransform(_tipWorldCoords);
    _trackedTip     = FemurToWorld.applyInverseTransform(_tipWorldCoords);
    _trackedTip     = _toFemurSTL.applyTransform(_trackedTip);

    _xAxis          = _DRBToTrackedPoint.applyTransform(plVector3(1, 0, 0));
    _xAxis          = DRBToWorld.applyTransform(_xAxis);
    _xAxis          = FemurToWorld.applyInverseTransform(_xAxis);
    _xAxis          = _toFemurSTL.applyTransform(_xAxis);
    _xAxis          = (_xAxis - _trackedTip).normalize();


    _yAxis          = _DRBToTrackedPoint.applyTransform(plVector3(0, 1, 0));
    _yAxis          = DRBToWorld.applyTransform(_yAxis);
    _yAxis          = FemurToWorld.applyInverseTransform(_yAxis);
    _yAxis          = _toFemurSTL.applyTransform(_yAxis);
    _yAxis          = (_yAxis - _trackedTip).normalize();

    _zAxis          = _DRBToTrackedPoint.applyTransform(plVector3(0, 0, 1));
    _zAxis          = DRBToWorld.applyTransform(_zAxis);
    _zAxis          = FemurToWorld.applyInverseTransform(_zAxis);
    _zAxis          = _toFemurSTL.applyTransform(_zAxis);
    _zAxis          = (_zAxis - _trackedTip).normalize();

    if (!_isArthroscope) 
    {
        _endWorldCoords = _DRBToTrackedEnd.applyTransform(_zeroVec);
    }
    else 
    {
        // arthroscope end is rotated by 30 degrees from camera view direction
        // and must be handled differently
        _endWorldCoords = _DRBToTrackedEnd.applyTransform(plVector3(0, 0, -10));
    }
    _endWorldCoords = DRBToWorld.applyTransform(_endWorldCoords);
    _trackedEnd     = FemurToWorld.applyInverseTransform( _endWorldCoords);
    _trackedEnd     = _toFemurSTL.applyTransform(_trackedEnd);

    _rotationAxis   = ( ((_trackedEnd-_trackedTip).normalize()) ^ plVector3(0,0,1) ).normalize();
    _rotationAngle  = - acos((_trackedEnd-_trackedTip).normalize() * plVector3(0,0,1)) * 57.295779513082320876798154;
}


void plTrackedObject::draw() const
{
    if ( _isArthroscope )
    {
        _drawScope();
    }    
    else
    {
        _drawProbe();
    }
}

void plTrackedObject::_drawScope() const
{
    // Draw the TA104 arthroscope
    static plShapeMesh c1(1.5f, 2.0f, 120.0f, 16, 4);
    static plShapeMesh c2(4.0f, 4.0f, 30.0f, 16, 4 );
    static plShapeMesh c3(8.0f, 8.0f, 60.0f, 16, 4 );
    static plShapeMesh c4(8.0f, 0.0f, 0.0f, 16, 4  );

    if ( _isVisible )
        plColourStack::load(0.4, 0.4, 0.4);
    else
        plColourStack::load(1.0, 0.25, 0.05);

    plModelStack::push();
    {
        plModelStack::translate( _trackedTip );
        plModelStack::rotate( _rotationAngle, _rotationAxis );
        c1.draw();
        plModelStack::push();
        {
            plModelStack::translate(0, 0, 120);
            c2.draw();
            plModelStack::push();
            {
                plModelStack::translate(0,0,30);
                c3.draw();
                plModelStack::translate(0,0,60);
                c4.draw();
            }
            plModelStack::pop();
        }    
        plModelStack::pop();
    }
    plModelStack::pop();
} 

void plTrackedObject::_drawProbe() const
{
    static plShapeMesh c1(0.0f, 1.0f, 4.0f, 16, 4);
    static plShapeMesh c2(1.0f, 2.5f, 124.0f, 16, 4);
    
    // Draw the TA002 probe
    if ( _isVisible )
        plColourStack::load(0.6, 0.6, 0.6);
    else
        plColourStack::load(1.0, 0.3, 0.1);
   
    plModelStack::push();
    {
        plModelStack::translate( _trackedTip );
        plModelStack::rotate( _rotationAngle, _rotationAxis );
        c1.draw();
        plModelStack::push();
        {
            plModelStack::translate(0, 0, 4);
            c2.draw();
        }
        plModelStack::pop();
    }
    plModelStack::pop();
}




