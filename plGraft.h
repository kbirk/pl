#ifndef __PL_GRAFT_H__
#define __PL_GRAFT_H__

#include "pl.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plGraftBase.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"

class plGraft : public plGraftBase,
                public plRenderable,
                public plEditable
{

    public:

        plGraft();
        
        //void readFromCSV( const plSeq<plString> &row );

        void adjustRadius( PLfloat adjustment) { plGraftBase::adjustRadius(adjustment); _updateCartilageMesh(); _updateBoneMesh(); } 
        void adjustLength( PLfloat adjustment) { plGraftBase::adjustLength(adjustment); _updateCartilageMesh(); _updateBoneMesh(); } 

        void draw() const;

        void setCaps();

    private:
    
        plMesh     _boneMesh;
        plMesh     _cartilageMesh;
        
        void      _setBoneColour() const;
        void      _setCartilageColour() const;
                
        void      _drawGraft() const;

        void      _updateCartilageMesh();
        void      _updateBoneMesh();
};

#endif
