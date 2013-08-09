#ifndef __PL_STAGE_0_SHADER_H__
#define __PL_STAGE_0_SHADER_H__

#include "plCommon.h"
#include "plShader.h"
#include "plSiteGrid.h"
#include "plSpline.h"

#define PL_MAX_GRAFT_CAP_TRIANGLES      500

class plPlannerStage0Shader : public plShader
{
    public:                    
          
        plPlannerStage0Shader ( const char *computeFile );
        ~plPlannerStage0Shader();

        void getUniformLocations();
        void setMeshSizeUniform ( PLuint size );
        
        //void bufferTextures( const plSiteGrid &grid );
        
        //PLfloat* dispatch();        
        
        //PLuint outputGraftCapsID()  const { return _outputGraftCapsID;  }  
        //PLuint outputGraftAreasID() const { return _outputGraftAreasID; }  
               
    private:
    
        // grid size (number of points/normals)
        
        //PLuint        _inputGridSizeID;
        
        // defectsite grid
        //PLuint        _inputGridSize;
        //PLuint        _inputGridPointsID; 
        //PLuint        _inputGridNormalsID; 
        // defectsite mesh
        PLuint        _siteMeshSizeID;  
        //PLuint        _inputSiteMeshSizeID;                  
        //PLuint        _inputSiteMeshTrianglesID;
        
        //PLfloat       _inputSiteArea;
        //PLuint        _inputSiteAreaID;
        //plVector3     _inputSiteNormal;
        //PLuint        _inputSiteNormalID;
        
        // possible graft caps and respective areas
        //PLuint        _outputGraftCapsID;
        //PLuint        _outputGraftAreasID;
};



#endif
