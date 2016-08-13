// volume.cpp
#include "plScanVolume.h"

// ============================================================ VOLUME ===================================================================

plScanVolume::plScanVolume() {
    voxels = nullptr;
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
    if (voxels != nullptr)
        delete[] voxels;
    //delete _minimalShader;
}

void plScanVolume::initializeVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW)
{
    // voxel information
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
    this->dimsV       = plVector3(PLfloat(ceil(dimsW.x/resolutionW)+1),
                                   PLfloat(ceil(dimsW.y/resolutionW)+1),
                                   PLfloat(ceil(dimsW.z/resolutionW)+1));
    this->arraySizeI  = dimsV.x*dimsV.y*dimsV.z;
    this->voxels      = new plScanVoxel[arraySizeI];

    // rendering stuff
    //this->_minimalShader = new plMinimalShader(PL_FILE_PREPATH"shaders/minimal.vert", PL_FILE_PREPATH"shaders/minimal.frag");

    this->updateBoundingBox();

    //renderMethod = CUBE;
}

void plScanVolume::updateBoundingBox()
{
    std::vector<plVector3> vertices;    vertices.reserve(8);
    std::vector<PLuint>    indices;     indices.reserve (8*3);

    vertices.push_back(plVector3(0.f     , 0.f     , dimsV.z));
    vertices.push_back(plVector3(dimsV.x , 0.f     , dimsV.z));
    vertices.push_back(plVector3(dimsV.x , dimsV.y , dimsV.z));
    vertices.push_back(plVector3(0.f     , dimsV.y , dimsV.z));
    vertices.push_back(plVector3(0.f     , 0.f     , 0.f));
    vertices.push_back(plVector3(dimsV.x , 0.f     , 0.f));
    vertices.push_back(plVector3(dimsV.x , dimsV.y , 0.f));
    vertices.push_back(plVector3(0.f     , dimsV.y , 0.f));

    indices.push_back(0);   indices.push_back(1);
    indices.push_back(1);   indices.push_back(2);
    indices.push_back(2);   indices.push_back(3);
    indices.push_back(3);   indices.push_back(0);
    indices.push_back(0);   indices.push_back(4);
    indices.push_back(1);   indices.push_back(5);
    indices.push_back(2);   indices.push_back(6);
    indices.push_back(3);   indices.push_back(7);
    indices.push_back(4);   indices.push_back(5);
    indices.push_back(5);   indices.push_back(6);
    indices.push_back(6);   indices.push_back(7);
    indices.push_back(7);   indices.push_back(4);

    // set vbo and attach attribute pointers
    std::shared_ptr<plVBO > vbo = std::make_shared<plVBO>();
    vbo->set(vertices);
    vbo->set(plVertexAttributePointer(PL_POSITION_ATTRIBUTE, sizeof(plVector4), sizeof(plVector4)));
    // set eabo
    std::shared_ptr<plEABO> eabo = std::make_shared<plEABO>();
    eabo->set(indices, GL_LINES);
    // attach to vao
    boundingBoxVAO.attach(vbo);
    boundingBoxVAO.attach(eabo);
    // upload to gpu
    boundingBoxVAO.upload();
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
    if (fmod(originTranslationW.x,resolutionW) != 0.f || fmod(originTranslationW.y,resolutionW) != 0.f || fmod(originTranslationW.z,resolutionW) != 0.f)
    {
        std::cerr << "Error in plScanVolume::resizeVolume(): the origin translation is not a multiple of the resolution. Aborting task." << std::endl;
        return false;
    }
    if (fmod(dimensionExpansionW.x,resolutionW) != 0.f || fmod(dimensionExpansionW.y,resolutionW) != 0.f || fmod(dimensionExpansionW.z,resolutionW) != 0.f)
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

    updateBoundingBox();

    return true;
}

/*PLuint plScanVolume::coordinatesWtoI(const plVector3& coordsW)
{
    // doesn't tkae into account negative coordinates and possibly causes invalid stuff
    return coordinatesVtoI(coordinatesWtoV(coordsW));
}*/

PLuint plScanVolume::coordinatesVtoI(const plVector3& coordsV) const // ASSUMES INTEGER COORDINATES!
{
    return (coordsV.x + dimsV.x * (coordsV.y + dimsV.y * coordsV.z));
}

plVector3 plScanVolume::coordinatesWtoV(const plVector3& coordsW) const
{
    PLint xV = round((coordsW.x - originW.x)/resolutionW);
    PLint yV = round((coordsW.y - originW.y)/resolutionW);
    PLint zV = round((coordsW.z - originW.z)/resolutionW);
    return plVector3(xV,yV,zV);
}

plVector3 plScanVolume::coordinatesVtoW(const plVector3& coordinatesV) const
{
    return (originW + plVector3(coordinatesV.x * resolutionW, coordinatesV.y * resolutionW, coordinatesV.z * resolutionW));
}

plVector3 plScanVolume::coordinatesItoW(PLuint index) const
{
    return coordinatesVtoW(coordinatesItoV(index));
}

plVector3 plScanVolume::coordinatesItoV(PLuint index) const
{
    PLuint xV = index%PLuint(dimsV.x);
    PLuint yV = ((index-xV)/PLuint(dimsV.x))%PLuint(dimsV.y);
    PLuint zV = index/PLuint(dimsV.y*dimsV.x);
    return plVector3(xV,yV,zV);
}

PLfloat plScanVolume::distanceWusingI(PLuint index1, PLuint index2) const
{
    return (coordinatesItoW(index1) - coordinatesItoW(index2)).length();
}

// ============================================================= MASK ====================================================================

plScanMask::plScanMask()
{
    voxels = nullptr;
}

plScanMask::plScanMask(PLfloat radiusW, PLfloat resolutionW)
{
    this->resolutionW = resolutionW;
    this->radiusW     = radiusW;
    this->radiusV     = PLuint(ceil(radiusW/resolutionW));

    plVector3 originW = plVector3(-PLfloat(radiusV)*resolutionW  ,-PLfloat(radiusV)*resolutionW  ,-PLfloat(radiusV)*resolutionW);
    plVector3 dimsW   = plVector3(PLfloat(radiusV)*resolutionW*2, PLfloat(radiusV)*resolutionW*2, PLfloat(radiusV)*resolutionW*2);

    initializeVolume(originW,dimsW,resolutionW);

    _assignMask();
}

void plScanMask::_assignMask()
{
    if (voxels == nullptr)
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
            PLfloat dist = ((coordinatesItoW(i) + offsets[j]) - middleVoxelCoordinatesW).length();
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
            surfaceIndices.push_back(i); // record this as a surface point (for quick lookup later)
        }
    }
}

// ============================================================= FIELD ===================================================================

plScanField::plScanField()
    : plScanVolume()
{
    voxels = nullptr;
}

plScanField::plScanField(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW)
    : plScanVolume (originW, dimensionsW, resolutionW) {}

plScanField::~plScanField()
{
    for (PLuint i = 0; i < masks.size(); i++)
        if (masks[i] != nullptr)
            delete masks[i];
}

PLbool plScanField::carveSphere(const plVector3& centreW, PLfloat radiusW)
{
    // first find or create the applicable mask
    PLuint maskIndex;
    for (maskIndex = 0; maskIndex < masks.size(); maskIndex++)
    {
        if (masks[maskIndex]->radiusW == radiusW)
            break;
    }
    if (maskIndex == masks.size())
    {
        masks.push_back(new plScanMask(radiusW,resolutionW));
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
    for (PLuint z = 0; z < masks[maskIndex]->radiusV*2+1; z++)
    {
        for (PLuint y = 0; y < masks[maskIndex]->radiusV*2+1; y++)
        {
            for (PLuint x = 0; x < masks[maskIndex]->radiusV*2+1; x++)
            {
                plVector3 coordinateOfVoxelVField = plVector3(x-PLint(masks[maskIndex]->radiusV),
                                                              y-PLint(masks[maskIndex]->radiusV),
                                                              z-PLint(masks[maskIndex]->radiusV)) + coordinatesV;
                PLuint indexOfVoxelField = coordinatesVtoI(coordinateOfVoxelVField);
                PLuint indexOfVoxelMask  = masks[maskIndex]->coordinatesVtoI(plVector3(x,y,z));
                if (!maskVoxel(indexOfVoxelField, indexOfVoxelMask, masks[maskIndex], centreW))
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
PLbool plScanField::maskVoxel(PLuint fieldVoxelIndex, PLuint maskVoxelIndex, const plScanMask* mask, const plVector3& pointW)
{
    voxels[fieldVoxelIndex].type = plScanVoxelType(voxels[fieldVoxelIndex].type & mask->voxels[maskVoxelIndex].type);

    if (mask->voxels[maskVoxelIndex].type == SURFACE) {
        voxels[fieldVoxelIndex].point.push_back(pointW);
    }

    return true;
}

// ============================================================= OTHER ===================================================================

std::ostream& operator << (std::ostream &stream, const plScanVolume &v)
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

std::ostream& operator << (std::ostream &stream, const plScanVolume::plScanVoxel &v)
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
