#include "plCheckerBoard.h"

plCheckerBoard::plCheckerBoard( PLfloat blocksize ) 
{           
    _generate( blocksize );
}


void plCheckerBoard::_generate( PLfloat blocksize )
{  
    
    PLuint  width_blocks  = 8;
    PLfloat width         = width_blocks * blocksize; 
      
    PLuint  height_blocks = 9;
    PLfloat height        = height_blocks * blocksize;
    
    PLbool black = true;

    plSeq<plVector3> vertices;
    plSeq<PLuint>    indices;

    for (PLint i = -1; i < width_blocks-1; i++)
    {
        PLfloat width_pos  = i * blocksize;

        for (PLint j = -1; j < height_blocks-1; j++)
        {        
            PLfloat height_pos = j * blocksize;
            
            plVector3 v0( width_pos, height_pos, 0.0f );
            plVector3 v1( width_pos+blocksize, height_pos, 0.0f );
            plVector3 v2( width_pos+blocksize, height_pos+blocksize, 0.0f );
            plVector3 v3( width_pos, height_pos+blocksize, 0.0f );
            
            plVector3 n = ( (v1-v0) ^ (v2-v0) ).normalize();
            
            plVector3 c = (black) ? plVector3(0,0,0) : plVector3(1,1,1); 
            
            PLuint base = vertices.size()/3;
            
            vertices.add( v0 ); vertices.add( n ); vertices.add( c );
            vertices.add( v1 ); vertices.add( n ); vertices.add( c );
            vertices.add( v2 ); vertices.add( n ); vertices.add( c );
            vertices.add( v3 ); vertices.add( n ); vertices.add( c );
            
            indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
            indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 );
                   
            black = !black;
        }
    }
    
    _mesh.setBuffers( vertices, indices );
}

void plCheckerBoard::draw() const
{      
    if ( !_isVisible )
        return;
   
    plModelStack::push( transform.matrix() );
    {
        _mesh.draw();
    }
    plModelStack::pop();
    
    
}
