#ifndef _PL_BOUNDARY_BASE_H_
#define _PL_BOUNDARY_BASE_H_

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"


class plBoundaryBase
{
    public:

        plBoundaryBase();       
          
        PLuint size() const;

        const plSeq<plVector3> &points () const { return _points;  } 
        const plSeq<plVector3> &normals() const { return _normals; }
        
        const plVector3        &points ( PLuint index ) const { return _points[index];  }
        const plVector3        &normals( PLuint index ) const { return _normals[index]; }

        plVector3 getAverageNormal() const;
        
        virtual void   readFromCSV( const plSeq<plString> &row );

        virtual PLuint addPointAndNormal   (const plVector3 &point, const plVector3 &normal);
        virtual void   movePointAndNormal  ( PLuint index, const plVector3 &point, const plVector3 &normal);
        virtual void   removePointAndNormal( PLuint index );

        friend std::ostream& operator << ( std::ostream& out, const plBoundaryBase &b );

    protected:
           
        plSeq<plVector3> _points;       // always in counterclockwise direction
        plSeq<plVector3> _normals;   

};

std::ostream& operator << ( std::ostream& out, const plBoundaryBase &b );

#endif

