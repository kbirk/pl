#ifndef __PL_RENDERER_H__
#define __PL_RENDERER_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plRenderingPipeline.h"

#include "plPhongShader.h"
#include "plMinimalShader.h"
#include "plPickingShader.h"
#include "plTextureShader.h"

#include "plPlan.h"
#include "plGraftEditor.h"
#include "plBoundaryEditor.h"
#include "plOctree.h"
#include "plTextureMesh.h"
#include "plAutomaticPlanner.h"
#include "plChessBoard.h"
#include "plScan.h"

/*
class plUniform
{

    public:
    
        PLuint numBytes;

};


class plRenderComponent
{

    public:
    
        //plShader *shader;
        PLuint      shaderType;
        plMesh     *mesh;
    
        //light position
        //light colour
        
        //picking values
        
        //view       transform
        //model      transform
        //projection transform

    
    private:

};
*/

//::queue( plRenderComponent( PL_PHONG_SHADER, &_sphere,  

namespace plRenderer
{  
    void init(); 
      
    void draw( PLuint x, PLuint y );  
     
    void queue ( const plPlan           &plan   );
    void queue ( const plGraftEditor    &editor );
    void queue ( const plBoundaryEditor &editor );
    void queue ( const plTrackedObject  &object );
    void queue ( const plTextureMesh    &arthroTexture );
    void queue ( const plChessBoard   &chessboard );
    void queue ( const plScan           &scan );
    
}











#endif

