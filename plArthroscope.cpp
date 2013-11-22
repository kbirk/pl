#include "plArthroscope.h"

plArthroscope::plArthroscope()
    :   _texture( ARTHRO_CAM_RES_X, ARTHRO_CAM_RES_Y, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE )
{
}


plArthroscope::plArthroscope(  const plDRBTransform &ToTrackedPoint, const plDRBTransform &ToTrackedEnd, const plDRBTransform &FemurDRBToFemurSTL )
    :   plTrackedObject( ToTrackedPoint, ToTrackedEnd, FemurDRBToFemurSTL, true ),
        _interpolationDeque( INTERPOLATION_LIMIT ),
        _isCameraView( false ),
        _texture( ARTHRO_CAM_RES_X, ARTHRO_CAM_RES_Y, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE )
{ 

    //_weights = { INTERPOLATION_WEIGHTS };

    _capture = cvCreateCameraCapture(0);
    if ( !_capture ) 
    {
        std::cerr << "Error: Unable to read from camera - aborting" << std::endl;
        exit(1);
    }

    // image distortion stuff
    _mapx = cvCreateImage( cvSize( CAPTURE_WIDTH, CAPTURE_HEIGHT ), IPL_DEPTH_32F, 1 );
    _mapy = cvCreateImage( cvSize( CAPTURE_WIDTH, CAPTURE_HEIGHT ), IPL_DEPTH_32F, 1 );
    _intrinsicsCV = (CvMat*) cvLoad("./data/matrices/Intrinsics.xml");
    _distortion   = (CvMat*) cvLoad("./data/matrices/Distortion.xml");
    std::cout << "Intrinsics and Distortion matrices loaded successfully." << std::endl;

    _image = cvCreateImage( cvSize( CAPTURE_WIDTH, CAPTURE_HEIGHT ), COLOUR_DEPTH, NUM_CHANNELS ); // added September 12 2011

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            _intrinsicsPL(row, col) = cvGetReal2D( _intrinsicsCV, row, col );
        }
    }

    _generateVAO();
}


plArthroscope::~plArthroscope() 
{
    cvReleaseCapture( &_capture );
    cvReleaseImage  ( &_image   );
}


const PLchar* plArthroscope::image() const
{
    return _image->imageData;
}


PLuint plArthroscope::width() const
{
    return _image->width;
}


PLuint plArthroscope::height() const
{
    return _image->height;
}


const plMatrix44& plArthroscope::intrinsics() const
{
    return _intrinsicsPL;
}


plMatrix44 plArthroscope::getProjectionMatrix() const
{
    const PLfloat x0        = 0.0f;
    const PLfloat y0        = 0.0f;
    const PLfloat zNear     = 10.0f;
    const PLfloat zFar      = 15000.0f;
    const PLfloat w         = (PLfloat) width();
    const PLfloat h         = (PLfloat) height();

    return plMatrix44 (2 * _intrinsicsPL(0,0) / w,-2 * _intrinsicsPL(0,1) / w , ( w  - 2 * _intrinsicsPL(0,2) + 2 * x0) / w , 0        ,
                       0                         , 2 * _intrinsicsPL(1,1) / h , (-h + 2 * _intrinsicsPL(1,2) + 2 * y0) / h  , 0        ,
                       0                         , 0                          , (-zFar - zNear)/(zFar - zNear)              ,-2*zFar*zNear/(zFar - zNear),
                       0                         , 0                          ,-1                                           , 0        );
}


plMatrix44 plArthroscope::getCameraMatrix() const
{
    // calculate vector down along the probe.
    plCamera scopeCamera;
    
    scopeCamera.position = getPoint();  // calculate vector down along the probe.
    scopeCamera.up       = -getAxisY();
    scopeCamera.lookat   = scopeCamera.position + getAxisZ();  // lookat position, scope tip + unit vector down along the probe
    
    return scopeCamera.getMatrix();
}


void plArthroscope::extractRenderComponents( plRenderMap& renderMap ) const
{
    if ( _isCameraView )
    {
        _extractCameraRenderComponents( renderMap );
    }
    else
    {
        _extractScopeRenderComponents( renderMap );
    }    
}


void plArthroscope::_generateVAO()
{
    std::vector<plVector3> vertices;        vertices.reserve( 8 );
    std::vector<PLuint>    indices;         indices.reserve( 6 );

    // position                                     // texture coord
    vertices.push_back( plVector3( -1, -1, 0 ) );   vertices.push_back( plVector3( 0,0,0) );
    vertices.push_back( plVector3(  1, -1, 0 ) );   vertices.push_back( plVector3( 1,0,0) );
    vertices.push_back( plVector3(  1,  1, 0 ) );   vertices.push_back( plVector3( 1,1,0) );
    vertices.push_back( plVector3( -1,  1, 0 ) );   vertices.push_back( plVector3( 0,1,0) );

    indices.push_back( 0 );   indices.push_back( 1 );   indices.push_back( 2 );
    indices.push_back( 0 );   indices.push_back( 2 );   indices.push_back( 3 );

    // set vbo and attach attribute pointers
    std::shared_ptr<plVBO> vbo( new plVBO() );
    vbo->set( vertices );
    vbo->set( plVertexAttributePointer( PL_POSITION_ATTRIBUTE, 0  ) );
    vbo->set( plVertexAttributePointer( PL_TEXCOORD_ATTRIBUTE, 16 ) );
    // set eabo
    std::shared_ptr<plEABO> eabo( new plEABO() );
    eabo->set( indices );

    // attach to vao
    _vao.attach( vbo );
    _vao.attach( eabo );
    // upload to gpu
    _vao.upload();
}


void plArthroscope::_extractCameraRenderComponents( plRenderMap& renderMap ) const
{
    static plMatrix44 ortho( -1, 1, -1, 1, -1, 1 );

    static plMatrix44 camera( 1, 0,  0, 0,
                              0, 1,  0, 0,
                              0, 0, -1, 0,
                              0, 0,  0, 1 ); 

    // draw walls
    // create render component
    plRenderComponent component( std::make_shared<plVAO>( _vao ) );
    // attached uniforms
    component.attach( plUniform( PL_MODEL_MATRIX_UNIFORM,      plMatrix44() ) );
    component.attach( plUniform( PL_VIEW_MATRIX_UNIFORM,       camera       ) );
    component.attach( plUniform( PL_PROJECTION_MATRIX_UNIFORM, ortho        ) );
    component.attach( plUniform( PL_TEXTURE_UNIT_0_UNIFORM,    &_texture    ) ); 
    // insert into render map
    renderMap[ PL_ARTHRO_CAM_TECHNIQUE ].insert( component );
}  

     
void plArthroscope::_extractScopeRenderComponents( plRenderMap& renderMap ) const
{
    if ( _isVisible )
        plColourStack::load(0.4, 0.4, 0.4);
    else
        plColourStack::load(1.0, 0.25, 0.05);

    plModelStack::push();
    {
        plModelStack::translate( _trackedTip );
        plModelStack::rotate( _rotationAngle, _rotationAxis );

        plRenderer::queue( plCone    ( PL_PLAN_TECHNIQUE, plVector3( 0, 0, 0 ), plVector3( 0, 0, 1 ), 1.5f, 2.0f, 120.0f ) );
        plRenderer::queue( plCylinder( PL_PLAN_TECHNIQUE, plVector3( 0, 0, 120 ), plVector3( 0, 0, 1 ), 4.0, 30.0f ) );
        plRenderer::queue( plCylinder( PL_PLAN_TECHNIQUE, plVector3( 0,0,150 ), plVector3( 0, 0, 1 ), 8.0, 60.0f ) );
        plRenderer::queue( plCone    ( PL_PLAN_TECHNIQUE, plVector3( 0,0,210 ), plVector3( 0, 0, 1 ), 8.0, 0.0f, 0.0f ) );
    }
    plModelStack::pop();
} 


void plArthroscope::updateImage( PLuint imageManipulation )
{
    // setting up the undistort map is really slow, we want to do it only once
    static bool firstTime = true;
    if (firstTime) 
    {
        cvInitUndistortMap( _intrinsicsCV, _distortion, _mapx, _mapy ); // SLOW SLOW SLOW
        firstTime = false;
    }

    _frame = cvQueryFrame( _capture );
    _image = _frame;

    // undistortion step
    if ( imageManipulation == CAMERA_IMAGE_UNDISTORT )
    {
//        _callCircle();
//        if ( _radius > 0 )
//        {
            IplImage *t = cvCloneImage( _image );
            cvRemap( t, _image, _mapx, _mapy ); // undistort image
            cvReleaseImage( &t );
            
            /*
            cv::Point center( _xCenter, _yCenter);
            cv::Mat dst;
            // center and radius are the results of HoughCircle
            // mask is a CV_8UC1 image with 0
            cv::Mat mask = cv::Mat::zeros( _frameMatrix.rows, _frameMatrix.cols, CV_8UC1 );
            circle( mask, center, _radius, cv::Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
            _frameMatrix.copyTo( dst, mask ); // copy values of img to dst if mask is > 0.
            dst.copyTo(frameMatrix);
            // circle( _frameMatrix, center, _radius, Scalar(0,255,00), 3, 8, 0 );
            */
//        }
    }
    
    _texture.set( _image->width, _image->height, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, _image->imageData );
}

/*
void plArthroscope::_callCircle()
{       
    _frameMatrix = _image;
    
    // convert to grayscale
    cv::Mat grayScale;
    cvtColor( _frameMatrix, grayScale, CV_BGR2GRAY );

    // Reduce the noise so we avoid false circle detection
    cv::GaussianBlur( grayScale, grayScale, cv::Size(9, 9), 2, 2 );  

    // Apply the Hough Transform to find the circles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles( grayScale, circles, CV_HOUGH_GRADIENT, 2, grayScale.rows/8, 200, 100, 356, 359 );

    //  Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center( cvRound( circles[i][0] ), cvRound( circles[i][1] ) );

        if ( interpolationDeque.size() == INTERPOLATION_LIMIT )
        {
            interpolationDeque.pop_back();        // pop off back if at limit
        }
            
        // add newest point  
        interpolationDeque.push_front( center );
            
        PLfloat finalX = 0;
        PLfloat finalY = 0;
        for( PLuint i = 0; i<interpolationDeque.size(); i++ )
        {
            finalX += interpolationDeque[i].x * _weights[i];
            finalY += interpolationDeque[i].y * _weights[i];
        }

        _xCenter = finalX;
        _yCenter = finalY;
        _radius = cvRound( circles[i][2] );
    }
}
*/
