#ifndef PL_CHESSBOARD_H
#define PL_CHESSBOARD_H

#include "plCommon.h"
#include "plTransparentRenderable.h"
#include "plRenderingPipeline.h"
#include "plTransform.h"
#include "plColourMesh.h"
#include "plDRBTransform.h"

class plChessBoard : public plTransparentRenderable
{

    public:
    
        plChessBoard();
        
        void updateTransform( const plDRBTransform &currentFemurDRBToWorld, const plDRBTransform &femurDRBToFemurSTL );
        
        void draw() const;

    private:

        plColourMesh _mesh;
        plTransform  _transform;
        
        plVector3   _calibOrigin, 
                    _calibXAxisPoint, 
                    _calibYAxisPoint;
        PLfloat     _blockSize;
        
        bool _readChessBoardCalib();
        void _generate();

};  




#endif
