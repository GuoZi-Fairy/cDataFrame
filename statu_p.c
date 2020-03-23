#include <stdlib.h>
#include "statu_p.h"
#include <assert.h>
#include <Windows.h>
#ifndef CSVREADER_H
    #define CSVREADER_H
    #include "csvReader.h"
#endif
#define STDCAL(type) _cdecl type
static STDCAL(void) check_coltype(column* col_obj)
{
    type ret = rnull;
    cell* cell_list = col_obj->series.cell_table;
    size_t length = col_obj->series.top / 2;//以表的前半部分为样本
    size_t iter;
    for(iter = 0; iter < length; iter++)
    {
        if(ret < cell_list[iter].type) ret = cell_list[iter].type;
    }
    col_obj->coltype = ret;
}

extern STDCAL(void) operate_trans(dataframe* df,dfbool calc_ability)
{
    size_t iter;
    size_t width = df->width;
    column* col_obj = df->col;
    if ( df->calculable == calc_ability) return;
    else 
    {
        column* col_ptr = col_obj;
        switch (calc_ability)
        {
            case dfFALSE:
                while (col_ptr != NULL)
                {
                    col_ptr->coltype = rnull;
                    col_ptr = col_ptr->next_col;
                }
                df->calculable = dfFALSE;
            break;

            case dfTRUE:
                while (col_ptr != NULL)
                {
                    check_coltype(col_ptr);
                    col_ptr = col_ptr->next_col;
                }
                df->calculable = dfTRUE;
            break;
        }
    }
}
/*********************************************************************/
/** 数值型操作{float,int} **/
#define DFSUM(COL_OBJ,SUM) do{\
    if(COL_OBJ->coltype == rint) SUM = __integerSUM__(COL_OBJ);\
    else if(COL_OBJ->coltype == rfloat) SUM = __floatSUM__(COL_OBJ);\
} while(0)
#define SFMEAN(COL_OBJ,MEAN) do{\
    if(COL_OBJ->coltype == rint) MEAN = __integerMEAN__(COL_OBJ);\
    else if(COL_OBJ->coltype == rfloat) MEAN = __floatMEAN__(COL_OBJ);\
} while(0)
static STDCAL(long long) __integerSUM__(column* col_obj)
{
    assert(col_obj->coltype == rint);
    cell* cellptr = col_obj->series.cell_table;
    size_t len = col_obj->series.top - 1;
    size_t iter = 0;
    long long ret = 0;
    for(iter = 0; iter < len; iter++)
    {
        if(cellptr[iter].type == rint) ret += cellptr[iter].data.integer_num;
        else continue;
    }
    return ret;
}
static STDCAL(double) __floatSUM__(column* col_obj)
{
    assert(col_obj->coltype == rfloat);
    cell* cellptr = col_obj->series.cell_table;
    size_t len = col_obj->series.top - 1;
    size_t iter = 0;
    double ret = 0;
    for(iter = 0; iter < len; iter++)
    {
       if(cellptr[iter].type == rfloat) ret += cellptr[iter].data.float_num;
       else continue;
    }
    return ret;
}
static STDCAL(double) __integerMEAN__(column* col_obj)
{
    assert(col_obj->coltype == rint);
    return ((double)__integerSUM__(col_obj))/(double)(col_obj->series.top -1 );
}
static STDCAL(double) __floatMEAN__(column* col_obj)
{
    assert(col_obj->coltype == rfloat);
    return __floatSUM__(col_obj)/(col_obj->series.top -1 );
}
extern STDCAL(cell) dfsum(dataframe* df,size_t index)
{
    assert(df->calculable == dfTRUE);
    cell ret = {0,rnull};
    column* col = colfind(df,index);
    if(col->coltype == rint)
    {
        long long sum = 0;
        ret.type = rint;
        DFSUM(col, sum);
        ret.data.integer_num = sum;
    }
    else if(col->coltype == rfloat)
    {
        double sum = 0.0;
        ret.type = rfloat;
        DFSUM(col, sum);
        ret.data.float_num = sum;
    }
    else assert(col->coltype == rint || col->coltype == rfloat);
    return ret;
}
/*********************************************************************/