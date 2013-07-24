#ifndef __PL_PICKING_SHADER_H__
#define __PL_PICKING_SHADER_H__

#include "plCommon.h"
#include "plMinimalShader.h"
#include "plPickingTexture.h"

class plPickingShader : public plMinimalShader
{
    public:                    
               
        plPickingShader(const char *vertexFile, const char *fragmentFile);
      
        void getUniformLocations();
		void setPickingUniforms(const plPickingInfo &pi) const;
		
    private:
	
		GLuint _redID;
		GLuint _greenID;
		GLuint _blueID;

};

#endif
