#ifndef PL_PICKING_SHADER_H
#define PL_PICKING_SHADER_H

#include "plCommon.h"
#include "plMinimalShader.h"

class plPickingShader : public plMinimalShader
{
    public:                    
               
        plPickingShader(const char *vertexFile, const char *fragmentFile);
      
        void getUniformLocations();
		void setPickingUniforms( PLint type, PLint id, PLint index ) const;
		
    private:
	
		GLuint _redID;
		GLuint _greenID;
		GLuint _blueID;

};

#endif
