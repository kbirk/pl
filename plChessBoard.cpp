#include "plChessBoard.h"

plChessBoard::plChessBoard( PLfloat blocksize )
{           
    _generate( blocksize );
    setTransparent();
    _readChessBoardCalib();
}


void plChessBoard::_generate( PLfloat blocksize )
{  
    
    PLint  width_blocks   = 8;
    PLfloat width         = width_blocks * blocksize; 
      
    PLint  height_blocks  = 9;
    PLfloat height        = height_blocks * blocksize;
    
    PLbool black = true;

    std::vector<plVector3> vertices;
    std::vector<PLuint>    indices;

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
            vertices.push_back( v0 ); vertices.push_back( n ); vertices.push_back( c );
            vertices.push_back( v1 ); vertices.push_back( n ); vertices.push_back( c );
            vertices.push_back( v2 ); vertices.push_back( n ); vertices.push_back( c );
            vertices.push_back( v3 ); vertices.push_back( n ); vertices.push_back( c );
            
            indices.push_back( base + 0 ); indices.push_back( base + 1 ); indices.push_back( base + 2 );
            indices.push_back( base + 0 ); indices.push_back( base + 2 ); indices.push_back( base + 3 );
            
        /* Commented out other faces of the 3D chessboard.
            base = vertices.size()/3;
            
            // back side 
            vertices.push_back( v1 + blocksize*-n); vertices.push_back( -n ); vertices.push_back( c );
            vertices.push_back( v0 + blocksize*-n); vertices.push_back( -n ); vertices.push_back( c );
            vertices.push_back( v3 + blocksize*-n); vertices.push_back( -n ); vertices.push_back( c );
            vertices.push_back( v2 + blocksize*-n); vertices.push_back( -n ); vertices.push_back( c );
            
            indices.push_back( base + 0 ); indices.push_back( base + 1 ); indices.push_back( base + 2 );
            indices.push_back( base + 0 ); indices.push_back( base + 2 ); indices.push_back( base + 3 ); 
                   
            black = !black;
            
            // left side
            if ( i == -1 )
            {
                base = vertices.size()/3;
             
                plVector3 leftNormal = ( n^(v3 - v0) ).normalize();
             
                vertices.push_back( v3 + blocksize*-n); vertices.push_back( leftNormal ); vertices.push_back( c );
                vertices.push_back( v0 + blocksize*-n); vertices.push_back( leftNormal ); vertices.push_back( c );
                vertices.push_back( v0 );               vertices.push_back( leftNormal ); vertices.push_back( c );
                vertices.push_back( v3 );               vertices.push_back( leftNormal ); vertices.push_back( c );
                
                indices.push_back( base + 0 ); indices.push_back( base + 1 ); indices.push_back( base + 2 );
                indices.push_back( base + 0 ); indices.push_back( base + 2 ); indices.push_back( base + 3 );  
            
            }

            // right side
            if ( i == width_blocks-2 )
            {
                base = vertices.size()/3;
             
                plVector3 rightNormal = ( n^(v1 - v2) ).normalize();
             
                vertices.push_back( v1 + blocksize*-n); vertices.push_back( rightNormal ); vertices.push_back( c );
                vertices.push_back( v2 + blocksize*-n); vertices.push_back( rightNormal ); vertices.push_back( c );
                vertices.push_back( v2 );               vertices.push_back( rightNormal ); vertices.push_back( c );
                vertices.push_back( v1 );               vertices.push_back( rightNormal ); vertices.push_back( c );
                
                indices.push_back( base + 0 ); indices.push_back( base + 1 ); indices.push_back( base + 2 );
                indices.push_back( base + 0 ); indices.push_back( base + 2 ); indices.push_back( base + 3 );  
            }
            
            // bottom side
            if ( j == -1 )
            {
                base = vertices.size()/3;
             
                plVector3 bottomNormal = ( n^(v0 - v1) ).normalize();
             
                vertices.push_back( v0 + blocksize*-n); vertices.push_back( bottomNormal ); vertices.push_back( c );
                vertices.push_back( v1 + blocksize*-n); vertices.push_back( bottomNormal ); vertices.push_back( c );
                vertices.push_back( v1 );               vertices.push_back( bottomNormal ); vertices.push_back( c );
                vertices.push_back( v0 );               vertices.push_back( bottomNormal ); vertices.push_back( c );
                
                indices.push_back( base + 0 ); indices.push_back( base + 1 ); indices.push_back( base + 2 );
                indices.push_back( base + 0 ); indices.push_back( base + 2 ); indices.push_back( base + 3 ); 
            }
            
            // top side
            if ( j == height_blocks-2 )
            {
                base = vertices.size()/3;
             
                plVector3 topNormal = ( n^(v2 - v3) ).normalize();
             
                vertices.push_back( v2 + blocksize*-n); vertices.push_back( topNormal ); vertices.push_back( c );
                vertices.push_back( v3 + blocksize*-n); vertices.push_back( topNormal ); vertices.push_back( c );
                vertices.push_back( v3 );               vertices.push_back( topNormal ); vertices.push_back( c );
                vertices.push_back( v2 );               vertices.push_back( topNormal ); vertices.push_back( c );
                
                indices.push_back( base + 0 ); indices.push_back( base + 1 ); indices.push_back( base + 2 );
                indices.push_back( base + 0 ); indices.push_back( base + 2 ); indices.push_back( base + 3 ); 
            }
        */
        }
    }
    
    _mesh.setBuffers( vertices, indices );
}


void plChessBoard::updateTransform( const plDRBTransform &currentFemurDRBToWorld, const plDRBTransform &femurDRBToFemurSTL )
{
    plVector3 chessOriginTrans = currentFemurDRBToWorld.applyInverseTransform( _calibOrigin );
    chessOriginTrans = femurDRBToFemurSTL.applyTransform( chessOriginTrans );

    plVector3 chessXTrans  = currentFemurDRBToWorld.applyInverseTransform( _calibXAxisPoint );
    chessXTrans  = femurDRBToFemurSTL.applyTransform(chessXTrans);

    plVector3 chessYTrans  = currentFemurDRBToWorld.applyInverseTransform( _calibYAxisPoint );
    chessYTrans  = femurDRBToFemurSTL.applyTransform(chessYTrans);

    plVector3 chessXAxis  = ( chessXTrans - chessOriginTrans ).normalize();
    plVector3 chessYAxis  = ( chessYTrans - chessOriginTrans ).normalize();

    /*
        the above axes aren't perfectly perpindicular (as they're generated from
        sampled points) so here we are forcing orthogonality.
            zAxis = xAxis x yAxis. 
            yAxis(fixed) = zAxis x xAxis
    */
    chessYAxis = ( (chessXAxis ^ chessYAxis) ^ chessXAxis );
    
    _transform.set( chessXAxis, chessYAxis, chessOriginTrans );
}
        

void plChessBoard::draw() const
{      
    if ( !_isVisible )
        return;
   
    plModelStack::push( _transform.matrix() );
    {
        if ( _isTransparent ) {
            plColourStack::load( PL_COLOUR_MESH_TRANSPARENT_COLOUR );
        }
        else {
            plColourStack::load( PL_COLOUR_MESH_OPAQUE_COLOUR );
        }


        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        _mesh.draw();
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    plModelStack::pop();
    
    
}


PLbool plChessBoard::_readChessBoardCalib()
{
    // This actually reads in three points, the origin and a step along each axis
    const char *chessBoardCalibFile = "data/registration/chessBoard";
    
    std::ifstream infile ( chessBoardCalibFile );
    
    if (!infile.good())
    {
        std::cerr << "plChessBoard::readChessBoardCalib() error: cannot open file, " << strerror(errno) << std::endl;
        return false;
    }
    
    plString line;
    std::getline(infile, line);
    if ( sscanf( line.c_str(), "%f %f %f", &_calibOrigin.x, &_calibOrigin.y, &_calibOrigin.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << chessBoardCalibFile << std::endl;
        return false;
    }
    std::getline(infile, line);
    if ( sscanf( line.c_str(), "%f %f %f", &_calibXAxisPoint.x, &_calibXAxisPoint.y, &_calibXAxisPoint.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << chessBoardCalibFile << std::endl;
        return false;
    }
    std::getline(infile, line);
    if ( sscanf( line.c_str(), "%f %f %f", &_calibYAxisPoint.x, &_calibYAxisPoint.y, &_calibYAxisPoint.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << chessBoardCalibFile << std::endl;
        return false;
    }

    std::cout << "Successfully read from: " << chessBoardCalibFile << std::endl;
    return true;

}
