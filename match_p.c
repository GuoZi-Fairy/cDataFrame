#include "dataframe.h"
#include "csvReader.h"
#include "statu_p.h"
#include <Windows.h>
// #define STDCAL(type) type __cdecl

typedef enum matchbool
{
    mFalse,
    mTrue,
}matchbool;
#define GETCELL(colobj,index) (colobj->series.cell_table + index)

#define __match_integer_(colobj,index) ((colobj->series.cell_table + index)->data.integer_num)
#define __match_float_(colobj,index) ((colobj->series.cell_table + index)->data.float_num)
#define MATCH(dataframe,index,expr,destn) do{\
    if(colfind(dataframe,index)->coltype == rint)\
    {\
        column* colobj = colfind(dataframe,index);\
        size_t iter = 0;\
        size_t iter_destn = 0;\
        while (iter < dataframe->length)\
        {\
            long x = __match_integer_(colobj,iter);\
            if(expr) destn[iter_destn++] = iter;\
            iter++;\
        }\
    }\
}while(0)
//destn应该是一个足够大(可以装下所有匹配项的索引)的整数型的线性数组
int main(int argc, char const *argv[])
{
    SetConsoleOutputCP(65001); 
    dataframe df = read_csv("test.csv");
    operate_trans(&df,dfTRUE);
    dfprint(&df,dfTRUE);
    size_t desn[1000] = {0};
    dataframe* dfobj = &df;
    MATCH(dfobj,0,x > 1000,desn);
    for(size_t i = 0; i< 1000;i++)printf("%lld\n",desn[i]);
    return 0;
}
