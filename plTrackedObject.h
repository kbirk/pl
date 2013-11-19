#ifndef TRACKED_OBJECT_H
#define TRACKED_OBJECT_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plDRBTransform.h"
#include "plRenderingPipeline.h"
#include "plVAO.h"
#include "plDraw.h"

class plTrackedObject : public plRenderable
{

    public:
           
        plTrackedObject ();                                                                           //  empty Constructor, allows for declaration of trackedObjects in headers        
        plTrackedObject ( const plDRBTransform &ToTrackedPoint, const plDRBTransform &ToTrackedEnd,   //  real constructor requiring three DRB transforms (and an arthro flag)
                          const plDRBTransform &FemurDRBToFemurSTL, bool isArthro = false);
                            
        void            updatePosition( const plDRBTransform &DRBToWorld, const plDRBTransform &FemurToWorld );

        // Single line accessors
        PLbool           isArthroscope()      const  { return _isArthroscope;   }
        const plVector3 &getPoint()           const  { return _trackedTip;      }
        const plVector3 &getEnd()             const  { return _trackedEnd;      }
        const plVector3 &getTipWorldCoords()  const  { return _tipWorldCoords;  }
        const plVector3 &getEndWorldCoords()  const  { return _endWorldCoords;  }
        const plVector3 &getRotationAxis()    const  { return _rotationAxis;    }
        PLfloat          getRotationAngle()   const  { return _rotationAngle;   }
        const plVector3 &getAxisX()           const  { return _xAxis;           }
        const plVector3 &getAxisY()           const  { return _yAxis;           }
        const plVector3 &getAxisZ()           const  { return _zAxis;           }
        plVector4        getRotationInfo()    const  { return plVector4( _rotationAxis, _rotationAngle); }

        void extractRenderComponents( std::set<plRenderComponent>& renderComponents ) const {}

        void draw() const;

    private:
    
        plDRBTransform   _DRBToTrackedPoint;
        plDRBTransform   _DRBToTrackedEnd;
        plDRBTransform   _FemurDRBtoSTL;

        plVector3       _zeroVec;
        plVector3       _trackedTip, _tipWorldCoords, _rotationAxis;
        plVector3       _trackedEnd, _endWorldCoords;
        plVector3       _xAxis, _yAxis, _zAxis;

        PLfloat         _rotationAngle;
        PLbool          _isArthroscope;
        
        void _drawScope() const;
        void _drawProbe() const;

};

#endif
