#ifndef STATU_P_H
    #include "csvReader.h"
    #include "dataframe.h"
    #define STATU_P_H
    typedef enum _boolean
    {
        dfFALSE,
        dfTRUE
    }dfbool;
    #define STDCAL(type) _cdecl type
    extern STDCAL(void) operate_trans(dataframe* df,dfbool calc_ability);
    extern STDCAL(cell) dfsum(dataframe* df,size_t index);
#endif