#ifndef __PL_CSV_H__
#define __PL_CSV_H__

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
