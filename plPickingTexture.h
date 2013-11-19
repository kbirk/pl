#ifndef PL_PICKING_H
#define	PL_PICKING_H

#include "plCommon.h"


class plPickingInfo 
{
    public:

	    PLint r;
	    PLint g;
	    PLint b;
	
	    plPickingInfo()
	        : r(-1), g(-1), b(-1)
	    {
	    }
	    
	    plPickingInfo( PLint red, PLint green, PLint blue) 
	        : r( red ), g( green ), b( blue )
	    {
	    }
};

  
class plPickingTexture
{
	public:
	
		plPickingTexture(GLuint width, GLuint height);

        const plPickingInfo &previousPick() const { return _previousPick; }

		void setFBO(PLuint width, PLuint height);
		void destroy();
		
		void bind();		
		void unbind();
		
		plPickingInfo readPixel(PLuint x, PLuint y);
        PLfloat       readDepth(PLuint x, PLuint y);
        
	private:
	
        PLbool        _readSinceLastDraw;
        plPickingInfo _previousPick;

		GLuint        _fbo;
		GLuint        _pickingTexture;
		GLuint        _depthStencilTexture;
};


namespace plPicking
{
    void init();
    
    const plPickingInfo& previousPick ();
    plPickingInfo        readPixel    ( PLuint x, PLuint y );

    void bind();
    void unbind();
    
    void resize( PLuint width, PLuint height );

}

#endif

