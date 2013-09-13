#include "plCheckerBoard.h"

plCheckerBoard::plCheckerBoard( PLfloat blocksize ) 
{           
    _generate( blocksize );
}


void plCheckerBoard::_generate( PLfloat blocksize )
{  
    
    PLint  width_blocks   = 8;
    PLfloat width         = width_blocks * blocksize; 
      
    PLint  height_blocks  = 9;
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
            
            // front side
            vertices.add( v0 ); vertices.add( n ); vertices.add( c );
            vertices.add( v1 ); vertices.add( n ); vertices.add( c );
            vertices.add( v2 ); vertices.add( n ); vertices.add( c );
            vertices.add( v3 ); vertices.add( n ); vertices.add( c );
            
            indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
            indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 );
            
            base = vertices.size()/3;
            
            // back side 
            vertices.add( v1 + blocksize*-n); vertices.add( -n ); vertices.add( c );
            vertices.add( v0 + blocksize*-n); vertices.add( -n ); vertices.add( c );
            vertices.add( v3 + blocksize*-n); vertices.add( -n ); vertices.add( c );
            vertices.add( v2 + blocksize*-n); vertices.add( -n ); vertices.add( c );
            
            indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
            indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 ); 
                   
            black = !black;
            
            // left side
            if ( i == -1 )
            {
                base = vertices.size()/3;
             
                plVector3 leftNormal = ( n^(v3 - v0) ).normalize();
             
                vertices.add( v3 + blocksize*-n); vertices.add( leftNormal ); vertices.add( c );
                vertices.add( v0 + blocksize*-n); vertices.add( leftNormal ); vertices.add( c );
                vertices.add( v0 );               vertices.add( leftNormal ); vertices.add( c );
                vertices.add( v3 );               vertices.add( leftNormal ); vertices.add( c );
                
                indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
                indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 );  
            
            }

            // right side
            if ( i == width_blocks-2 )
            {
                base = vertices.size()/3;
             
                plVector3 rightNormal = ( n^(v1 - v2) ).normalize();
             
                vertices.add( v1 + blocksize*-n); vertices.add( rightNormal ); vertices.add( c );
                vertices.add( v2 + blocksize*-n); vertices.add( rightNormal ); vertices.add( c );
                vertices.add( v2 );               vertices.add( rightNormal ); vertices.add( c );
                vertices.add( v1 );               vertices.add( rightNormal ); vertices.add( c );
                
                indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
                indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 );  
            }
            
            // bottom side
            if ( j == -1 )
            {
                base = vertices.size()/3;
             
                plVector3 bottomNormal = ( n^(v0 - v1) ).normalize();
             
                vertices.add( v0 + blocksize*-n); vertices.add( bottomNormal ); vertices.add( c );
                vertices.add( v1 + blocksize*-n); vertices.add( bottomNormal ); vertices.add( c );
                vertices.add( v1 );               vertices.add( bottomNormal ); vertices.add( c );
                vertices.add( v0 );               vertices.add( bottomNormal ); vertices.add( c );
                
                indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
                indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 ); 
            }
            
            // top side
            if ( j == height_blocks-2 )
            {
                base = vertices.size()/3;
             
                plVector3 topNormal = ( n^(v2 - v3) ).normalize();
             
                vertices.add( v2 + blocksize*-n); vertices.add( topNormal ); vertices.add( c );
                vertices.add( v3 + blocksize*-n); vertices.add( topNormal ); vertices.add( c );
                vertices.add( v3 );               vertices.add( topNormal ); vertices.add( c );
                vertices.add( v2 );               vertices.add( topNormal ); vertices.add( c );
                
                indices.add( base + 0 ); indices.add( base + 1 ); indices.add( base + 2 );
                indices.add( base + 0 ); indices.add( base + 2 ); indices.add( base + 3 ); 
            }
            
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
        if ( _isTransparent )
            plColourStack::load( PL_COLOUR_MESH_TRANSPARENT_COLOUR ); 
        else
            plColourStack::load( PL_COLOUR_MESH_OPAQUE_COLOUR ); 
            
        _mesh.draw();
    }
    plModelStack::pop();
    
    
}


void plCheckerBoard::toggleVisibility()
{
    if (_isTransparent) 
    {
        _isVisible = false;
        _isTransparent = false;
    } 
    else if (!_isVisible) 
    {
        _isVisible = true;
        _isTransparent = false;
    } 
    else 
    {
        _isVisible = true;
        _isTransparent = true;
    }
}


void plCheckerBoard::toggleTransparency()
{
    if (_isTransparent)
    {
        _isVisible = true;
        _isTransparent = false;
    }
    else
    {
        _isVisible = true;
        _isTransparent = true;
    }
}


PLbool readCheckerBoardCalib( plVector3 &origin, plVector3 &xStep, plVector3 &yStep )
{
    // This actually reads in three points, the origin and a step along each axis
    const char * checkerBoardCalibFile = "data/registration/checkerBoard";
    
    std::ifstream infile ( checkerBoardCalibFile );
    
    if (!infile.good())
    {
        std::cerr << "plCheckerBoard::readCheckerBoardCalib() error: cannot open file, " << strerror(errno) << std::endl;
        return false;
    }
    
    plString line;
    std::getline(infile, line);
    if ( sscanf( line.c_str(), "%f %f %f", &origin.x, &origin.y, &origin.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << checkerBoardCalibFile << std::endl;
        return false;
    }
    std::getline(infile, line);
    if ( sscanf( line.c_str(), "%f %f %f", &xStep.x, &xStep.y, &xStep.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << checkerBoardCalibFile << std::endl;
        return false;
    }
    std::getline(infile, line);
    if ( sscanf( line.c_str(), "%f %f %f", &yStep.x, &yStep.y, &yStep.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << checkerBoardCalibFile << std::endl;
        return false;
    }

    std::cout << "Successfully read from: " << checkerBoardCalibFile << std::endl;
    return true;

}
