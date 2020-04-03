#ifndef CSVREADER_H
    #define CSVREADER_H
    #include <stdio.h>
    #include "dataframe.h"
    #include "error.h"
    #include "header.h"
    extern STDCAL(dataframe) read_csv(const char* path);
    extern STDCAL(void) df_to_csv(dataframe* df,const char* path);
#endif