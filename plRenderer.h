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




class plRenderer
{
    public:
               
        static void init(); 
          
        static void draw();  
         
        static void queue ( const plPlan           &plan   );
        static void queue ( const plGraftEditor    &editor );
        static void queue ( const plBoundaryEditor &editor );
        static void queue ( const plTrackedObject  &object ); 
        static void queue ( const plTextureMesh    &arthroTexture );
         
        static void reportError ( const plString &str );

    private: 

        plRenderer() {};

        static const plPlan             *_planToDraw;
        static const plGraftEditor      *_graftEditorToDraw;
        static const plBoundaryEditor   *_boundaryEditorToDraw;         
        static const plTextureMesh      *_arthroTextureToDraw;
              
        static plSeq<const plTrackedObject*> _trackedObjectsToDraw;
        
        static plMinimalShader  *_minimalShader;
        static plPhongShader    *_phongShader;
        static plPickingShader  *_pickingShader; 
        static plTextureShader  *_textureShader;
        
        static void _setOpenGLState();
        
        static void _beginPicking();
        static void _endPicking();
        
        static void _beginDrawing();
        static void _endDrawing();
        
        static void _drawScene();
        static void _drawScenePicking();

        static void _clearRenderQueue();
        
        static void _drawArthroTexture();
        
};











#endif

