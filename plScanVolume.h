// volume.h
#ifndef PL_SCAN_VOLUME_H
#define PL_SCAN_VOLUME_H

#include <cmath>
#include "plVector3.h"
#include "plCommon.h"
#include "plVAO.h"
#include "plRenderable.h"

const float probeRadius   = 1.5;             // 1.5 mm radius
const float coneHalfAngle = 60.0/180.0*PL_PI; // 60 degree half-angle

class plScanVolume //: public plRenderable
{
    //plMinimalShader* _minimalShader;

    public:

        enum plScanVoxelType { EMPTY = 0x0, SURFACE = 0x1, UNKNOWN = 0x3 }; // Note that AND of two types yields the correct type
        //enum plRenderMethod { DISK, CUBE, TRIANGLES, VALUES }; // Method to render a voxel

        class plScanVoxel
        {
            public:

                std::vector<plVector3> normal; // all normals at this voxel
                std::vector<plVector3> point;  // all surface points at this voxel
                plScanVoxelType  type;   // EMPTY, SURFACE, or UNKNOWN

                plScanVoxel()
                {
                    type = UNKNOWN;
                }

                //PLbool draw( const plVector3& coordV ) const;

        };

        // attributes
        PLfloat           resolutionW;    // the increase in world units (mm) along each voxel in each dimension
        plVector3         originW;        // coordinates of the center of voxels[0][0][0] in world units (mm)
        plVector3         dimsW;          // dimensions of volume in world units (mm)

        plVector3         dimsV;          // dimensions of volume in voxels

        PLuint            arraySizeI;     // size of the voxel array

        plScanVoxel*      voxels;         // voxels[x][y][z] is indexed as voxels[ x + dimx * (y + dimy * z) ]

        plVAO             boundingBoxVAO;

        //plRenderMethod renderMethod;    // how voxels are rendered

        // methods
        plScanVolume();
        plScanVolume(const plScanVolume&);
        plScanVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW);
        virtual ~plScanVolume();

        void      initializeVolume(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW);
        void      updateBoundingBox();
        PLbool    enlargeVolume   (const plVector3& originTranslationW, const plVector3& dimensionExpansionW);
        //PLuint    coordinatesWtoI(const plVector3& coordsW);
        PLuint    coordinatesVtoI(const plVector3& coordsV)      const;
        plVector3 coordinatesItoW(PLuint index)                  const;
        plVector3 coordinatesVtoW(const plVector3& coordinatesV) const;
        plVector3 coordinatesItoV(PLuint index)                  const;
        plVector3 coordinatesWtoV(const plVector3& coordsW)      const;
        PLfloat   distanceWusingI(PLuint index1, PLuint index2)  const;

        //PLbool    draw() const;

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
        std::vector<PLuint> surfaceIndices;
        PLfloat       maxAngleForSurface;

        // methods
        plScanMask();
        plScanMask(PLfloat radiusW, PLfloat resolutionW);
};


class plScanField : public plScanVolume
{
    public:

        // attributes
        std::vector<plScanMask*> masks;          // masks to AND with volume when carving out a sphere

        // methods
        plScanField();
        plScanField(const plVector3& originW, const plVector3& dimensionsW, PLfloat resolutionW);
        ~plScanField();

        PLbool carveSphere( const plVector3& centreW, PLfloat radiusW );
        PLbool maskVoxel( PLuint fieldVoxelIndex, PLuint maskVoxelIndex, const plScanMask* mask, const plVector3& pointW );
};

std::ostream& operator << ( std::ostream &stream, const plScanVolume &v );
std::ostream& operator << ( std::ostream &stream, const plScanVolume::plScanVoxel &v );

#endif
