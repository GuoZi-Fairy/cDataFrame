#include <stdio.h>
#ifndef DATAFRAME_H 
    #define DATAFRAME_H
    #include "dataframe.h"
#endif
#define CSVREADER_H
#define CSVREADER_STDCAL(type) type __cdecl
extern CSVREADER_STDCAL(void) dfprint(dataframe* df,int preview);
extern CSVREADER_STDCAL(void) colprint(column* col_obj,char sep);
extern CSVREADER_STDCAL(void) cellprint(cell* cell_obj);
extern CSVREADER_STDCAL(cell*) cellfind(dataframe* df,size_t col_index,size_t index);
extern CSVREADER_STDCAL(dataframe) read_csv(const char* path);
extern CSVREADER_STDCAL(char*) colname(column* col_obj,size_t index);
extern CSVREADER_STDCAL(column*) colfind(dataframe* df,size_t index);//获取df中的第index个col对象
extern CSVREADER_STDCAL(void) dffree(dataframe* df);