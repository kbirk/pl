// volume.h
#ifndef PL_SCAN_VOLUME_H
#define PL_SCAN_VOLUME_H

#include <fstream>
#include "plVector3.h"
#include "plCommon.h"
#include "plSeq.h"
//#include "plRenderable.h"

const float probeRadius   = 1.5;             // 1.5 mm radius
const float coneHalfAngle = 60.0/180.0*M_PI; // 60 degree half-angle

class plScanVolume //: public plRenderable
{
 public:

    enum plScanVoxelType { EMPTY = 0x0, SURFACE = 0x1, UNKNOWN = 0x3 }; // Note that AND of two types yields the correct type
    //enum plRenderMethod { DISK, CUBE, TRIANGLES, VALUES }; // Method to render a voxel

    class plScanVoxel {
      public:

        plSeq<plVector3> normal; // all normals at this voxel
        plSeq<plVector3> point;  // all surface points at this voxel
        plScanVoxelType  type;   // EMPTY, SURFACE, or UNKNOWN

        plScanVoxel() {
            type = UNKNOWN;
        }

    };

  // attributes
    PLfloat           resolutionW;    // the increase in world units (mm) along each voxel in each dimension
    plVector3         originW;        // coordinates of the center of voxels[0][0][0] in world units (mm)
    plVector3         dimsW;          // dimensions of volume in world units (mm)

    plVector3         dimsV;          // dimensions of volume in voxels

    PLuint            arraySizeI;     // size of the voxel array

    plScanVoxel*      voxels;         // voxels[x][y][z] is indexed as voxels[ x + dimx * (y + dimy * z) ]
    //plRenderMethod renderMethod;    // how voxels are rendered

  // methods
    plScanVolume();
    plScanVolume(const plScanVolume&);
    plScanVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW);
    virtual ~plScanVolume();

    void      initializeVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW);
    PLbool    enlargeVolume(const plVector3& originTranslationW, const plVector3& dimensionExpansionW);
    //PLuint    coordinatesWtoI(const plVector3& coordsW);
    PLuint    coordinatesVtoI(const plVector3& coordsV);
    plVector3 coordinatesItoW(PLuint index);
    plVector3 coordinatesVtoW(const plVector3& coordinatesV);
    plVector3 coordinatesItoV(PLuint index);
    plVector3 coordinatesWtoV(const plVector3& coordsW);
    PLfloat   distanceWusingI(PLuint index1, PLuint index2);

    plScanVolume& operator=(const plScanVolume&);
};

class plScanMask : public plScanVolume
{
  private:
    void _assignMask();

  public:
  // attributes
    PLfloat       radiusW;
    PLuint        radiusV;
    plSeq<PLuint> surfaceIndices;

  // methods
    plScanMask();
    plScanMask(PLfloat radiusW, PLfloat resolutionW);
};

class plScanField : public plScanVolume
{
  public:
  // attributes
    plSeq<plScanMask*> masks;          // masks to AND with volume when carving out a sphere

  // methods
    plScanField();
    plScanField(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW);
    ~plScanField();

    PLbool carveSphere(const plVector3& centreW, PLfloat radiusW);
    PLbool maskVoxel( PLuint voxelIndex, plScanVoxelType type, const plVector3& point );
};

std::ostream& operator << ( std::ostream &stream, const plScanVolume &v );
std::ostream& operator << ( std::ostream &stream, const plScanVolume::plScanVoxel &v );

#endif
