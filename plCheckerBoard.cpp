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


// This actually reads in three points, the origin and a step along each axis
PLbool readCheckerBoardCalib( plVector3 &origin, plVector3 &xStep, plVector3 &yStep )
{
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
    
    /*
    FILE *fileIn;
    const char * checkerBoardCalibFile = "data/registration/checkerBoard";
    char oneLine[255];

    fileIn = fopen(checkerBoardCalibFile, "r");
    if (fileIn == NULL)
    {
        std::cerr << "Error opening " << checkerBoardCalibFile << " -- " << strerror(errno) << std::endl;
        return false;
    }
    fgets(oneLine, 255, fileIn);
    if ( sscanf(oneLine, "%f %f %f", &origin.x, &origin.y, &origin.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << checkerBoardCalibFile << std::endl;
        return false;
    }
    fgets(oneLine, 255, fileIn);
    if ( sscanf(oneLine, "%f %f %f", &xAxis.x, &xAxis.y, &xAxis.z ) != 3)
    {
        std::cerr << "Invalid base calibration file: " << checkerBoardCalibFile << std::endl;
        return false;
    }
    fgets(oneLine, 255, fileIn);
    if ( sscanf(oneLine, "%f %f %f", &yAxis.x, &yAxis.y, &yAxis.z ) != 3 )
    {
        std::cerr << "Invalid base calibration file: " << checkerBoardCalibFile << std::endl;
        return false;
    }
    fclose(fileIn);
    std::cout << "Successfully read from: " << checkerBoardCalibFile << std::endl;
    return true;
    */
}
