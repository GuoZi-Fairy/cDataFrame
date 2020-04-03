#include<stdio.h>
#include"dataframe.h"
#include"error.h"
#include <assert.h>
#include <stdlib.h>
#include "header.h"
#define COLLIST_LEN 200
#define CUTOFF_LINE "------------------------------------------------------------------------------"
#define OMIT_LINE ".............................................................................."
static STDCAL(column*) get_column(column* col_obj,size_t index)
{
    get_column_recursion://用于取消递归
    assert(col_obj!=NULL);
    if (col_obj->index!=index)
    {
        col_obj = col_obj->next_col;
        goto get_column_recursion;
    }
    else return col_obj;
}
static STDCAL(void) foreach_cellprint(dataframe* df, column* collist[200], size_t length, size_t width)
{
    size_t wide,len;
    for(len = 0;len < length;len++)
    {
        for(wide =0;wide < width;wide++)
        {
            cell* cell_obj = collist[wide]->series.cell_table + len;
            cellprint(cell_obj);
            putchar('\t');
        }
        putchar('\n');
    }
}
extern STDCAL(void) cellprint(cell* cell_obj)//输出一个cell
{
    if(cell_obj==NULL)
    {
        printf("NULL CELL");
    }
    switch(cell_obj->type)
    {
        case rint:
            printf("%lld",cell_obj->data.integer_num);
            break;
        case rfloat:
            printf("%lf",cell_obj->data.float_num);
            break;
        case rchar:
            printf("%s",cell_obj->data.char_ch);
            break;
        case rnan:
            printf("NAN");
            break;
        case rnull:
            RAISE(_cell_type_error);
    }
}
extern STDCAL(void) columns_free(column* columns)
{
    if (columns!=NULL)
    {
        columns_free(columns->next_col);
        free(columns->series.cell_table);
        free(columns);
    }
}
extern STDCAL(void) dffree(dataframe* df)
{
    column* columns = df->col;
    if (columns!=NULL)
    {
        columns_free(columns->next_col);
        free(columns->series.cell_table);
        free(columns);
    }
}
extern STDCAL(column*) colfind(dataframe* df,size_t index)
{
    column* ret;

    ret = get_column(df->col,index);
    return ret;
}
extern STDCAL(cell*) cellfind(dataframe* df,size_t col_index,size_t index)
{
    cell* ret;
    column* col_obj = get_column(df->col,col_index);
    ret = col_obj->series.cell_table + index;
    return ret;

}
extern STDCAL(char*) colname(column* col_obj,size_t index)//获取col序列中第index个元素的name
{
    get_column_name_recursion://用于取消递归
    assert(col_obj!=NULL);
    if (col_obj->index!=index)
    {
        col_obj = col_obj->next_col;
        goto get_column_name_recursion;
    }
    else return col_obj->name;
}
extern STDCAL(void) colprint(column* col_obj,char sep)//输出col序列
{
    while(col_obj!=NULL)
    {
        printf("%s%c",col_obj->name,sep);
        col_obj = col_obj->next_col;
    }
    putchar('\n');
}

extern STDCAL(void) dfprint(dataframe* df,int preview)//预览  只有0 或 非0
{
    size_t wide,len;
    column* col_list[COLLIST_LEN] = {NULL};
    column* col_obj = df->col;//拷贝一份col,注:该指针会发生改变,若需链表头，可直接访问df->col
    size_t iter;
    for(iter = 0;col_obj!=NULL;iter++)
    {
        col_list[iter] = col_obj;
        col_obj = col_obj->next_col;//此处为col_obj发生改变的地方
    }
    colprint(df->col,'\t');
    if(preview){
        foreach_cellprint(df,col_list,5,iter);
        printf(OMIT_LINE);
    }
    else foreach_cellprint(df,col_list,df->length,iter);
    printf("\n[%lldx%lld]\n",df->length,df->width);
    printf(CUTOFF_LINE);
    putchar('\n');

}