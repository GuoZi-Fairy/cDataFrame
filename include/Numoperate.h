#ifndef CSVREADER_H
    #define CSVREADER_H
    #include "csvReader.h"
    #include "header.h"
#endif
typedef enum _num_type
{
    num_float,
    num_int
}num_type;
typedef struct _large_num
{
    char value[1024];//使用小端法
    num_type _type;
}dfnum;