#ifndef MATCH_P_H
    #define MATCH_P_H
    #include "dataframe.h"
    #include "csvReader.h"
    #include "statu_p.h"
    #include <assert.h>
    #define __match_integer_(colobj,index) ((colobj->series.cell_table + index)->data.integer_num)
    #define __match_float_(colobj,index) ((colobj->series.cell_table + index)->data.float_num)
    /*
    MATCH
    * dataframe 一个指向dataframe的指针
    * index dataframe中列的索引
    * expr 一个合法的逻辑判断表达式,其中允许有一个变量x ,x的值是列中每个单元的值
    * destn 一个足够大的size_t类型的线性数组.
    */
    #define MATCH(dataframePtr,index,expr,destn) do{\
        if(colfind(dataframePtr,index)->coltype == rint)\
        {\
            column* colobj = colfind(dataframePtr,index);\
            size_t iter = 0;\
            size_t iter_destn = 0;\
            while (iter < dataframePtr->length)\
            {\
                long x = __match_integer_(colobj,iter);\
                if(!!(expr)) destn[iter_destn++] = iter;\
                iter++;\
            }\
        }\
        else if(colfind(dataframePtr,index)->coltype == rfloat)\
            {\
            column* colobj = colfind(dataframePtr,index);\
            size_t iter = 0;\
            size_t iter_destn = 0;\
            while (iter < dataframePtr->length)\
            {\
                double x = __match_float_(colobj,iter);\
                if(!!(expr)) destn[iter_destn++] = iter;\
                iter++;\
            }\
        }\
        else assert(colfind(dataframePtr,index)->coltype == rint || colfind(dataframePtr,index)->coltype == rfloat);\
    }while(0)
#endif