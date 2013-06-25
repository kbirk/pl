#ifndef CSV_H
#define CSV_H

#include "pl.h"
#include "plSeq.h"
#include "stringhelper.h"

class plCSV 
{
    public:

        plSeq< plSeq<plString> > data;       
        plString                 filename;

        plCSV( plString fn );

    private:

        void readFile( plString filename, PLbool verbose = false );

};  


#endif
