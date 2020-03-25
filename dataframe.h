// 基础数据结构
#ifndef DATAFRAME_H
    #define DATAFRAME_H
    #define COL_NAME_LENGTH_MAX 100
    #include <stdio.h>
    typedef enum type //sortation means the priority
    {
        rnull,
        rnan,
        rint,
        rfloat,
        rchar,
    }type;
    typedef union _element //cell的数据用此类型
    {
        char char_ch[200];//不能超过99个字节
        long long integer_num;
        double float_num;
        char nan[4];
    }element;

    typedef struct _cell //一个单元
    {   
        element data;
        type type;
    }cell;
    typedef struct _series //一个单元序列
    {
        cell* cell_table;
        size_t size;
        size_t top;
    }series;
    typedef struct _columns //列的序列
    {
        size_t index;
        char name[COL_NAME_LENGTH_MAX];
        struct _columns* next_col;
        series series;
        type coltype;
    }column;
    typedef struct _dataframe //数据表
    {
        size_t length;
        size_t width;
        column* col;
        char calculable;
    }dataframe;
#endif