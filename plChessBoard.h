#ifndef PL_CHESSBOARD_H
#define PL_CHESSBOARD_H

#include "plCommon.h"
#include "plTransparentRenderable.h"
#include "plRenderingPipeline.h"
#include "plTransform.h"
#include "plVAO.h"
#include "plDRBTransform.h"
#include "plRenderer.h"

class plChessBoard : public plTransparentRenderable
{

    public:
    
        plChessBoard();
        
        void updateTransform( const plDRBTransform &currentFemurDRBToWorld, const plDRBTransform &femurDRBToFemurSTL );
        
        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
    private:

        std::shared_ptr< plVAO > _vao;
        plTransform  _transform;
        
        plVector3   _calibOrigin, 
                    _calibXAxisPoint, 
                    _calibYAxisPoint;
        PLfloat     _blockSize;
        
        bool _readChessBoardCalib();
        void _generateVAO();

};  




#endif
