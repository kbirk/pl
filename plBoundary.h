#ifndef PL_BOUNDARY_H
#define PL_BOUNDARY_H

#include "plCommon.h"
#include "plMath.h"

#include "plVector3.h"
#include "plRenderable.h"
#include "plEditable.h"
#include "plPicking.h"
#include "plBoneAndCartilage.h"
#include "plVAO.h"
#include "plDraw.h"
#include "plSphere.h"
#include "plRenderer.h"

#define PL_SELECTED_BOUNDARY_POINT_RADIUS   1.0f
#define PL_BOUNDARY_POINT_RADIUS            0.75f


class plBoundary : public plRenderable,
                   public plEditable
{
    public:

        plBoundary();      
        plBoundary( const std::vector<plString> &row );
 
        PLuint size() const;

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractEditorRenderComponents( plRenderMap& renderMap ) const;
        
        const plVector3& points ( PLuint index ) const { return _points[index];  }
        const plVector3& normals( PLuint index ) const { return _normals[index]; }

        plVector3 getAverageNormal() const;

        virtual PLuint addPointAndNormal   ( const plVector3 &point, const plVector3 &normal);
        virtual void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal);
        virtual void   removePointAndNormal( PLuint index );
        virtual void   clear();            
        
    protected:

        std::vector<plVector3> _points;       // always in counterclockwise direction
        std::vector<plVector3> _normals;   
        
        std::shared_ptr< plVAO > _vao;    

        plVector4 _getColour() const;    
        
        void _generateVAO(); 
        
        void _extractPointRenderComponents( plRenderMap& renderMap ) const;
        void _extractPointEditorRenderComponents( plRenderMap& renderMap ) const;
        
};

std::ostream& operator << ( std::ostream& out, const plBoundary &b );

#endif

