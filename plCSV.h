#ifndef PL_CSV_H
#define PL_CSV_H

#include "plCommon.h"
#include "plSeq.h"
#include "plString.h"

class plCSV 
{
    public:

        plSeq< plSeq<plString> > data;       
        plString                 filename;

        plCSV( plString fn );

    private:

        void _readFile( plString filename, PLbool verbose = false );

};  


#endif
