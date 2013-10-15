// volume.cpp
#include "plScanVolume.h"

// ============================================================ VOLUME ===================================================================

plScanVolume::plScanVolume() {
    voxels = NULL;
}

plScanVolume::plScanVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW) {
    initializeVolume(originW,dimensionsW,resolutionW);
}

plScanVolume::plScanVolume(const plScanVolume& other)
{
    this->initializeVolume(other.originW,other.dimsW,other.resolutionW);
    for (PLuint i = 0; i < arraySizeI; i++)
    {
        this->voxels[i] = other.voxels[i];
    }
}

plScanVolume& plScanVolume::operator=(const plScanVolume& other)
{
    this->initializeVolume(other.originW,other.dimsW,other.resolutionW);
    for (PLuint i = 0; i < arraySizeI; i++)
    {
        this->voxels[i] = other.voxels[i];
    }
    return (*this);
}

plScanVolume::~plScanVolume()
{
    if (voxels != NULL)
        delete[] voxels;
}

void plScanVolume::initializeVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW)
{
    this->resolutionW = resolutionW;
    this->originW     = originW;
    if (dimensionsW.x <= 0.f || dimensionsW.y <= 0.f || dimensionsW.z <= 0.f)
    {
        std::cerr << "Error in plScanVolume::plScanVolume(): the dimensions provided as input to plScanVolume are negative. Using (1 1 1)" << std::endl;
        this->dimsW   = plVector3(1,1,1);
    }
    else
    {
        this->dimsW   = dimensionsW;
    }
    this->dimsV       = plVector3( PLfloat( ceil(dimsW.x/resolutionW)+1 ),
                                   PLfloat( ceil(dimsW.y/resolutionW)+1 ),
                                   PLfloat( ceil(dimsW.z/resolutionW)+1 ) );
    this->arraySizeI  = dimsV.x*dimsV.y*dimsV.z;
    this->voxels = new plScanVoxel[arraySizeI];

    //renderMethod = CUBE;
}

PLbool plScanVolume::enlargeVolume(const plVector3 &originTranslationW, const plVector3 &dimensionExpansionW)
{
    // check inputs
    if (originTranslationW.x > 0.f || originTranslationW.y > 0.f || originTranslationW.z > 0.f)
    {
        std::cerr << "Error in plScanVolume::resizeVolume(): the origin translation is not zero or negative in all directions. Aborting task" << std::endl;
        return false;
    }
    if (dimensionExpansionW.x < 0.f || dimensionExpansionW.y < 0.f || dimensionExpansionW.z < 0.f)
    {
        std::cerr << "Error in plScanVolume::resizeVolume(): the dimension expansion is not zero or positive in all directions. Aborting task" << std::endl;
        return false;
    }
    if ( fmod(originTranslationW.x,resolutionW) != 0.f || fmod(originTranslationW.y,resolutionW) != 0.f || fmod(originTranslationW.z,resolutionW) != 0.f)
    {
        std::cerr << "Error in plScanVolume::resizeVolume(): the origin translation is not a multiple of the resolution. Aborting task." << std::endl;
        return false;
    }
    if ( fmod(dimensionExpansionW.x,resolutionW) != 0.f || fmod(dimensionExpansionW.y,resolutionW) != 0.f || fmod(dimensionExpansionW.z,resolutionW) != 0.f)
    {
        std::cerr << "Error in plScanVolume::resizeVolume(): the origin translation is not a multiple of the resolution. Aborting task." << std::endl;
        return false;
    }

    // set up updated attributes, but do not set yet.
    plVector3    newDimsW = dimsW + dimensionExpansionW;
    plVector3    newDimsV = dimsV + plVector3((dimensionExpansionW.x/resolutionW),(dimensionExpansionW.y/resolutionW),(dimensionExpansionW.z/resolutionW));
    plVector3    newOriginW = originW + originTranslationW;
    PLuint       newArraySizeI = newDimsV.x * newDimsV.y * newDimsV.z;

    plScanVoxel* newVoxels = new plScanVoxel[newArraySizeI];

    // move contents of old array into new array
    for (PLuint i = 0; i < arraySizeI; i++)
    {
        PLuint newI(0);
        plVector3 coordinateV(coordinatesItoV(i));
        newI += PLuint((coordinateV.z-originTranslationW.z)*(newDimsV.x*newDimsV.y));
        newI += PLuint((coordinateV.y-originTranslationW.y)*(newDimsV.x));
        newI += PLuint((coordinateV.x-originTranslationW.x));
        newVoxels[newI] = voxels[i];
    }

    // update volume attributes
    dimsW      = newDimsW;
    dimsV      = newDimsV;
    originW    = newOriginW;
    arraySizeI = newArraySizeI;

    delete[] voxels;
    voxels = newVoxels;

    return true;
}

/*PLuint plScanVolume::coordinatesWtoI(const plVector3& coordsW)
{
    // doesn't tkae into account negative coordinates and possibly causes invalid stuff
    return coordinatesVtoI(coordinatesWtoV(coordsW));
}*/

PLuint plScanVolume::coordinatesVtoI(const plVector3& coordsV) // ASSUMES INTEGER COORDINATES!
{
    return (coordsV.x + dimsV.x * ( coordsV.y + dimsV.y * coordsV.z) );
}

plVector3 plScanVolume::coordinatesWtoV(const plVector3& coordsW)
{
    PLint xV = round( (coordsW.x - originW.x)/resolutionW );
    PLint yV = round( (coordsW.y - originW.y)/resolutionW );
    PLint zV = round( (coordsW.z - originW.z)/resolutionW );
    return plVector3(xV,yV,zV);
}

plVector3 plScanVolume::coordinatesVtoW(const plVector3& coordinatesV)
{
    return (originW + plVector3(coordinatesV.x * resolutionW, coordinatesV.y * resolutionW, coordinatesV.z * resolutionW));
}

plVector3 plScanVolume::coordinatesItoW(PLuint index)
{
    return coordinatesVtoW(coordinatesItoV(index));
}

plVector3 plScanVolume::coordinatesItoV(PLuint index)
{
    PLuint xV = index%PLuint(dimsV.x);
    PLuint yV = ((index-xV)/PLuint(dimsV.x))%PLuint(dimsV.y);
    PLuint zV = index/PLuint(dimsV.y*dimsV.x);
    return plVector3(xV,yV,zV);
}

PLfloat plScanVolume::distanceWusingI(PLuint index1, PLuint index2)
{
    return (coordinatesItoW(index1) - coordinatesItoW(index2)).length();
}

PLbool plScanVolume::draw() const
{
    return true;
}

// ============================================================= MASK ====================================================================

plScanMask::plScanMask()
{
    voxels = NULL;
}

plScanMask::plScanMask(PLfloat radiusW, PLfloat resolutionW)
{
    this->resolutionW = resolutionW;
    this->radiusW     = radiusW;
    this->radiusV     = PLuint(ceil(radiusW/resolutionW));

    plVector3 originW = plVector3(-PLfloat(radiusV)*resolutionW  ,-PLfloat(radiusV)*resolutionW  ,-PLfloat(radiusV)*resolutionW  );
    plVector3 dimsW   = plVector3( PLfloat(radiusV)*resolutionW*2, PLfloat(radiusV)*resolutionW*2, PLfloat(radiusV)*resolutionW*2);

    initializeVolume(originW,dimsW,resolutionW);

    _assignMask();
}

void plScanMask::_assignMask()
{
    if (voxels == NULL)
    {
        std::cerr << "Error in plScanMask::_assignMask(), voxels have not been assigned yet. This is indicative of a big problem. Aborting." << std::endl;
        return;
    }

    // Fill mask with UNKNOWN / SURFACE / EMPTY tags
    const plVector3 offsets[8] = {   // offsets to the corners of a voxel
      plVector3(-0.5,-0.5,-0.5), plVector3(-0.5,-0.5,+0.5), plVector3(-0.5,+0.5,-0.5), plVector3(-0.5,+0.5,+0.5),
      plVector3(+0.5,-0.5,-0.5), plVector3(+0.5,-0.5,+0.5), plVector3(+0.5,+0.5,-0.5), plVector3(+0.5,+0.5,+0.5)
    };
    plVector3 middleVoxelCoordinatesW = coordinatesItoW(arraySizeI/2);
    for (PLuint i=0; i<arraySizeI; i++) {

        // Find the min and max distances to points in this voxel (check only the corners)
        PLfloat maxDist = 0;
        PLfloat minDist = 9999;
        for (int j=0; j<8; j++) {
            PLfloat dist = ( (coordinatesItoW(i) + offsets[j]) - middleVoxelCoordinatesW ).length();
            if (dist < minDist) minDist = dist;
            if (dist > maxDist) maxDist = dist;
        }

        // Fill in data for this mask voxel
        if (maxDist < radiusW) { // this voxel is inside the probe
            voxels[i].type = EMPTY;
        } else if (radiusW < minDist) { // this voxel is outside the probe
            voxels[i].type = UNKNOWN;
        } else { // this voxel intersects the probe surface
            voxels[i].type = SURFACE;
            surfaceIndices.add(i); // record this as a surface point (for quick lookup later)
        }
    }
}

// ============================================================= FIELD ===================================================================

plScanField::plScanField()
    : plScanVolume()
{
    voxels = NULL;
}

plScanField::plScanField(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW)
    : plScanVolume (originW, dimensionsW, resolutionW ) {}

plScanField::~plScanField()
{
    for (PLuint i = 0; i < masks.size(); i++)
        if (masks[i] != NULL)
            delete masks[i];
}

PLbool plScanField::carveSphere(const plVector3& centreW, PLfloat radiusW)
{
    // first find or create the applicable mask
    PLuint maskIndex;
    for (maskIndex = 0; maskIndex < masks.size(); maskIndex++ )
    {
        if (masks[maskIndex]->radiusW == radiusW)
            break;
    }
    if (maskIndex == masks.size())
    {
        masks.add(new plScanMask(radiusW,resolutionW));
    }

    // second find the nearest voxel coordinate to the supposed physical location of the centre.
    plVector3 coordinatesV(coordinatesWtoV(centreW));

    // third resize the volume if necessary to fit the mask
    plVector3 originTranslationW(0,0,0);
    plVector3 dimensionExpansionW(0,0,0);
    PLbool    volumeEnlargeRequired(false);
    while (coordinatesV.x - masks[maskIndex]->radiusV < originTranslationW.x/resolutionW)
    {
        originTranslationW.x  = -masks[maskIndex]->radiusW*5.f;
        dimensionExpansionW.x += masks[maskIndex]->radiusW*5.f;
        volumeEnlargeRequired = true;
    }
    while (coordinatesV.y - masks[maskIndex]->radiusV < originTranslationW.y/resolutionW)
    {
        originTranslationW.y  = -masks[maskIndex]->radiusW*5.f;
        dimensionExpansionW.y += masks[maskIndex]->radiusW*5.f;
        volumeEnlargeRequired = true;
    }
    while (coordinatesV.z - masks[maskIndex]->radiusV < originTranslationW.z/resolutionW)
    {
        originTranslationW.z  = -masks[maskIndex]->radiusW*5.f;
        dimensionExpansionW.z += masks[maskIndex]->radiusW*5.f;
        volumeEnlargeRequired = true;
    }
    while (coordinatesV.x + masks[maskIndex]->radiusV >= dimsV.x + dimensionExpansionW.x/resolutionW)
    {
        dimensionExpansionW.x += masks[maskIndex]->radiusW*5.f;
        volumeEnlargeRequired = true;
    }
    while (coordinatesV.y + masks[maskIndex]->radiusV >= dimsV.y + dimensionExpansionW.y/resolutionW)
    {
        dimensionExpansionW.y += masks[maskIndex]->radiusW*5.f;
        volumeEnlargeRequired = true;
    }
    while (coordinatesV.z + masks[maskIndex]->radiusV >= dimsV.z + dimensionExpansionW.z/resolutionW)
    {
        dimensionExpansionW.z += masks[maskIndex]->radiusW*5.f;
        volumeEnlargeRequired = true;
    }
    if (volumeEnlargeRequired)
    {
        if (!enlargeVolume(originTranslationW,dimensionExpansionW))
            return false; // error detected
        coordinatesV = (coordinatesWtoV(centreW)); // this will need to be updated
    }

    // fourth iterate through the mask and apply it to all voxels
    for (PLint z = 0; z < masks[maskIndex]->radiusV*2+1; z++)
    {
        for (PLint y = 0; y < masks[maskIndex]->radiusV*2+1; y++)
        {
            for (PLint x = 0; x < masks[maskIndex]->radiusV*2+1; x++)
            {
                plVector3 coordinateOfVoxelVField = plVector3(x-PLint(masks[maskIndex]->radiusV),
                                                              y-PLint(masks[maskIndex]->radiusV),
                                                              z-PLint(masks[maskIndex]->radiusV)) + coordinatesV;
                PLuint indexOfVoxelField = coordinatesVtoI(coordinateOfVoxelVField);
                PLuint indexOfVoxelMask  = masks[maskIndex]->coordinatesVtoI(plVector3(x,y,z));
                if (!maskVoxel( indexOfVoxelField, masks[maskIndex]->voxels[indexOfVoxelMask].type, centreW))
                    return false; // error detected
            }
        }
    }

    return true;
}

// Mask one voxel
//
// For surfaces, store the surface point that is farthest through the
// voxel (i.e.  that contains most of the voxel to its interior).
PLbool plScanField::maskVoxel( PLuint voxelIndex, plScanVoxelType mask, const plVector3& point )
{
    voxels[voxelIndex].type = plScanVoxelType(voxels[voxelIndex].type & mask);

    if (mask == SURFACE) {
        voxels[voxelIndex].point.add(point);
    }

    return true;
}

// ============================================================= OTHER ===================================================================

std::ostream& operator << ( std::ostream &stream, const plScanVolume &v )
{
    PLuint i(0);
    for (PLuint z = 0; z < v.dimsV.z; z++)
    {
        for (PLuint y = 0; y < v.dimsV.y; y++)
        {
            for (PLuint x = 0; x < v.dimsV.x; x++)
            {
                stream << v.voxels[i] << " ";
                i++;
            }
            stream << "\n";
        }
        stream << "\n\n\n";
    }
    return stream;
}

std::ostream& operator << ( std::ostream &stream, const plScanVolume::plScanVoxel &v )
{
    switch (v.type)
    {
    case plScanVolume::EMPTY:
        stream << "E";
        break;
    case plScanVolume::UNKNOWN:
        stream << "U";
        break;
    case plScanVolume::SURFACE:
        stream << "S";
        break;
    default: // really shouldn't happen...
        stream << "_";
        break;
    }
    return stream;
}

#if 0
// Carve a sphere out of the volume

void Volume::carveSphere( vector centre, vector dir )

{
  const int nVoxelsPerDim = VOLUME_DIM / VOXEL_SIZE;
  const int memForVolume = rint( nVoxelsPerDim * nVoxelsPerDim * nVoxelsPerDim * sizeof(Voxel) / 1024.0 / 1024.0 );

  const int radiusMaskVoxels  = (int) ceil( probeRadius / VOXEL_SIZE ); // number of MaskVoxels on radius of mask
  const int nMaskVoxelsPerDim = radiusMaskVoxels * 2 + 1; // 2 radii, 1 for centre
  const int nMaskVoxels       = nMaskVoxelsPerDim * nMaskVoxelsPerDim * nMaskVoxelsPerDim; // memory required for mask

  const vector offsets[8] = {   // offsets to the corners of a voxel
    vector(-0.5,-0.5,-0.5), vector(-0.5,-0.5,+0.5), vector(-0.5,+0.5,-0.5), vector(-0.5,+0.5,+0.5),
    vector(+0.5,-0.5,-0.5), vector(+0.5,-0.5,+0.5), vector(+0.5,+0.5,-0.5), vector(+0.5,+0.5,+0.5)
  };

  // Store previous point (for building cylinders between subsequent points)

  bool havePreviousPoint = (voxels != NULL);
  static vector previousPoint;
  static vector previousDir;

  // First time through: Create voxel array and mask

  if (voxels == NULL) 
  {

    // Create voxel array

    if (memForVolume > 512)     // warn if volume is > 0.5 GB
      cout << memForVolume << " MB of voxels" << endl;

    voxels = new Voxel[ nVoxelsPerDim * nVoxelsPerDim * nVoxelsPerDim ];
    dims   = (VOLUME_DIM/VOXEL_SIZE) * vector(1,1,1);
    origin = centre - (0.5 * VOLUME_DIM) * vector(1,1,1);

    // Create mask

    mask = new MaskVoxel[ nMaskVoxels ];

    // Fill mask with UNKNOWN / SURFACE / EMPTY tags

    int i = 0;

    int nEmpty = 0;
    int nUnknown = 0;

    
    for (int z=-radiusMaskVoxels; z<=radiusMaskVoxels; z++)
      for (int y=-radiusMaskVoxels; y<=radiusMaskVoxels; y++)
        for (int x=-radiusMaskVoxels; x<=radiusMaskVoxels; x++) {

	  // Find the min and max distances to points in this voxel (check only the corners)

	  float maxDist = 0;
	  float minDist = 9999;
	  for (int j=0; j<8; j++) {
	    float dist = VOXEL_SIZE * ( vector(x,y,z) + offsets[j] ).length();
	    if (dist < minDist) minDist = dist;
	    if (dist > maxDist) maxDist = dist;
	  }

	  // Fill in data for this mask voxel

	  mask[i].dir = vector(x,y,z).normalize();
	  
	  if (maxDist < probeRadius) { // this voxel is inside the probe
	    mask[i].type = EMPTY;
	    nEmpty++;
	  } else if (probeRadius < minDist) { // this voxel is outside the probe
	    mask[i].type = UNKNOWN;
	    nUnknown++;
	  } else { // this voxel intersects the probe surface 
	    mask[i].type = SURFACE;
	    surfacePoints.add( MaskSurfacePoint( mask[i].dir, i ) ); // record this as a surface point (for quick lookup later)
	  }
	  
	  i++;
	}

    // cout << "Mask has " << surfacePoints.size() << " surface, " << nEmpty << " empty, and " << nUnknown << " unknown voxels." << endl;
  }


  // Carve out the sphere from the voxel array


  // FIRST: Change the mask SURFACE points to EMPTY points inside the cone

  int nEmpty = 0;

  for (int i=0; i<surfacePoints.size(); i++) {

    // Find angle between this surface point and the cone axis
    
    float sine   = (surfacePoints[i].dir ^ dir).length();
    float cosine = surfacePoints[i].dir * dir;
    float angle  = atan2( sine, cosine );

    // If this surface point is inside, change it to EMPTY

    if (angle < coneHalfAngle) {
      surfacePoints[i].inCone = true;
      mask[ surfacePoints[i].index ].type = EMPTY;
      nEmpty++;
    }
  }

  // cout << nEmpty << " of " << surfacePoints.size() << " surface points are empty" << endl;

  // SECOND: Mask the volume

  // Find the location in the volume of mask[0][0][0]

  vector roundedCentre( rint((centre.x - origin.x)/VOXEL_SIZE)*VOXEL_SIZE+origin.x,
			rint((centre.y - origin.y)/VOXEL_SIZE)*VOXEL_SIZE+origin.y,
			rint((centre.z - origin.z)/VOXEL_SIZE)*VOXEL_SIZE+origin.z );

  vector zeroPos = roundedCentre - (radiusMaskVoxels * VOXEL_SIZE) * vector(1,1,1); // spatial location of mask[0][0][0] (rounded to a voxel centre)
  vector offset  = (1.0 / VOXEL_SIZE) * (zeroPos - origin); // volume[offset.x][offset.y][offset.z] is mask[0][0][0]

  int ox = (int) rint( offset.x );
  int oy = (int) rint( offset.y );
  int oz = (int) rint( offset.z );

  if (ox >= 0 && ox+nMaskVoxelsPerDim <= dims.x && // only consider masks that fall completely inside the volume
      oy >= 0 && oy+nMaskVoxelsPerDim <= dims.y &&
      oz >= 0 && oz+nMaskVoxelsPerDim <= dims.z) {

    // Mask the volume

    int index = ox + dims.x * (oy + dims.y * oz);

    Voxel *voxelP = &voxels[index];
    MaskVoxel *maskP = &mask[0];

    for (int z=0; z<nMaskVoxelsPerDim; z++) {
      for (int y=0; y<nMaskVoxelsPerDim; y++) {
	for (int x=0; x<nMaskVoxelsPerDim; x++) {
	  maskVoxel( voxelP, maskP->type, roundedCentre + probeRadius * maskP->dir, maskP->dir );
	  voxelP++;
	  maskP++;
	}
	voxelP += (int) (dims.x - nMaskVoxelsPerDim);
      }
      voxelP += (int) (dims.x * (dims.y - nMaskVoxelsPerDim));
    }
  }

  // THIRD: Restore the mask EMPTY points to SURFACE points

  for (int i=0; i<surfacePoints.size(); i++)
    if (surfacePoints[i].inCone) {
      surfacePoints[i].inCone = false;
      mask[ surfacePoints[i].index ].type = SURFACE;
    }

  // FOURTH: Fill in the cylinder between this and the previous point

  if (havePreviousPoint)
    coverWithCylinder( previousPoint, roundedCentre, previousDir, dir );

  previousPoint = roundedCentre;
  previousDir = dir;

  // Flag that the data has changed so that we later recompute the surface

  mustComputeTriangles = true;
}


// Mask one voxel
//
// For surfaces, store the surface point that is farthest through the
// voxel (i.e.  that contains most of the voxel to its interior).

void Volume::maskVoxel( Voxel *voxelP, VoxelType type, vector point, vector dir )

{

  VoxelType prevType = voxelP->type;
	  
  voxelP->type = (VoxelType) (voxelP->type & type);
	  
  if (voxelP->type == SURFACE) {

    if (prevType != SURFACE) { // first point

      voxelP->normal = dir;
      voxelP->point  = point;

    } else {		// additional point

      // test whether each point is to the outside of the tangent plane of the other point

      bool newIsOutsideOld = ((point - voxelP->point) * voxelP->normal > 0);
      bool oldIsOutsideNew = ((voxelP->point - point) * dir > 0);

      if (newIsOutsideOld && !oldIsOutsideNew) {

	voxelP->normal = dir;
	voxelP->point  = point;
		  
      } else if (newIsOutsideOld && oldIsOutsideNew || !newIsOutsideOld && !oldIsOutsideNew) {

	voxelP->normal = (voxelP->normal + dir).normalize();
	voxelP->point  = 0.5 * (voxelP->point + point);
      }
    }
  }
}   


// Draw the volume

void Volume::draw()

{
  if (mustComputeTriangles)
    computeTriangles();

  Voxel *voxelP = voxels;
  FieldPoint *valueP = values;

  GLUquadricObj *quadric = gluNewQuadric();

  int nSurface = 0;
  int nEmpty = 0;

  if (renderMethod == TRIANGLES && mustComputeTriangles)
    computeTriangles();

  for (int z=0; z<dims.z; z++)
    for (int y=0; y<dims.y; y++)
      for (int x=0; x<dims.x; x++) {
	
	if (voxelP->type == SURFACE) {

	  vector pos;

	  if (useVoxelCentre)
	    pos = origin + VOXEL_SIZE * vector(x,y,z); // point is at centre of voxel
	  else
	    pos = voxelP->point; // point is average surface point inside this voxel 

	  vector norm = -1 * voxelP->normal;

	  if (voxelP->selected) {
	    glPushMatrix();
	    glTranslatef( pos.x, pos.y, pos.z );
	    gluSphere( quadric, VOXEL_SIZE * 0.1, 10, 10 );
	    glPopMatrix();
	  }
	  
	  glColor4f( 0.2, 0.7, 0.4, 1.0 );

	  switch (renderMethod) {

	  case DISK:		// Render a surface voxel as a disk
	    {
	      vector xAxis = diskSize * VOXEL_SIZE * norm.perp1();
	      vector yAxis = diskSize * VOXEL_SIZE * norm.perp2();

	      glBegin( GL_TRIANGLE_FAN );

	      glNormal3fv( &norm.x );
	      glVertex3fv( &pos.x );

	      for (float theta=0; theta<=2*M_PI; theta+=M_PI/180.0*30) {
		vector p = pos + cos(theta) * xAxis + sin(theta) * yAxis;
		glVertex3fv( &p.x );
	      }

	      glEnd();
	    }
	    break;

	  case VALUES:

	    for (int i=-1; i<1; i++)
	      for (int j=-1; j<1; j++)
		for (int k=-1; k<1; k++) {
		  
		  FieldPoint *corner = valueP + (int) (i + dims.x * (j + dims.y * k));

		  float dist = fabs(corner->value) / (2*VOXEL_SIZE);
		  if (dist > 1) dist = 1;

		  if (fabs(corner->value) < 0.005)
		    glColor3f( 0.1, 0.1, 0.7 );
		  else if (corner->value < 0)
		    glColor3f( dist, 1, dist );
		  else
		    glColor3f( 1, dist, dist );

		  glPushMatrix();
		  glTranslatef( corner->pos.x, corner->pos.y, corner->pos.z );
		  gluSphere( quadric, VOXEL_SIZE * 0.1, 10, 10 );
		  glPopMatrix();
		}
	    break;

	  case CUBE:		// Render as a cube

	    if (voxelP->triangles != NULL && voxelP->triangles->size() > 0) {
	      glPushMatrix();
	      glTranslatef( pos.x, pos.y, pos.z );
	      glutSolidCube( diskSize * VOXEL_SIZE );
	      glPopMatrix();
	    }
	    break;

	  case TRIANGLES:

	    if (voxelP->triangles != NULL) {
	      glBegin( GL_TRIANGLES );
	      for (int i=0; i<voxelP->triangles->size(); i++) {
		Triangle &tri = (*(voxelP->triangles))[i];
		glNormal3fv( &tri.n.x );
		glVertex3fv( &tri.v0.x );
		glVertex3fv( &tri.v1.x );
		glVertex3fv( &tri.v2.x );
	      }
	      glEnd();
	    }
	    break;
	  }

	  nSurface++;

	} else if (voxelP->type == EMPTY)

	  nEmpty++;
	  
	voxelP++;
	valueP++;
      }

  // cout << "\r" << nSurface << " surface voxels,  " << nEmpty << " empty voxels"; cout.flush();
}
#endif

