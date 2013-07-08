#ifndef __PL_PICKING_FBO_H
#define	__PL_PICKING_FBO_H

#include "pl.h"

class plPickingInfo 
{
    public:

	    PLint type;
	    PLint id;
	    PLint index;
	
	    plPickingInfo()
	    {
		    type   = 0;
		    id     = 0;
		    index  = 0;
	    }
};

  
class plPickingTexture
{
	public:
	
		plPickingTexture(GLuint width, GLuint height);
		~plPickingTexture();

        const plPickingInfo &lastPick() const { return _lastPick; }

		void init(PLuint width, PLuint height);
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

#endif

