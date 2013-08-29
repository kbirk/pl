#ifndef __PL_PICKING_H__
#define	__PL_PICKING_H__

#include "plCommon.h"


class plPickingInfo 
{
    public:

	    PLint type;
	    PLint id;
	    PLint index;
	
	    plPickingInfo()
	        : type(0), id(0), index(0)
	    {
	    }
};

  
class plPickingTexture
{
	public:
	
		plPickingTexture(GLuint width, GLuint height);

        const plPickingInfo &lastPick() const { return _lastPick; }

		void setFBO(PLuint width, PLuint height);
		void destroy();
		
		void bind();		
		void unbind();
		
		plPickingInfo readPixel(PLuint x, PLuint y);
        PLfloat       readDepth(PLuint x, PLuint y);
        
	private:
	
        PLbool        _readSinceLastDraw;
        plPickingInfo _lastPick;

		GLuint        _fbo;
		GLuint        _pickingTexture;
		GLuint        _depthStencilTexture;
};


namespace plPicking
{
    //public:

        extern plPickingInfo     value;
        extern plPickingTexture *texture;
        
        void init();
        
    //private:
    
        //plPicking() {};   
};

#endif

