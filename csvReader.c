//TODO:
#include <stdlib.h>
#include <stdio.h>
#include "csvReader.h"
#include <assert.h>
#include <string.h>
#include <Windows.h>
#define STDCAL(type) type __cdecl

typedef struct parser
{
    const char* text;
    char* stack;
    size_t top;
    size_t size;
}parser;
typedef enum _bool_csv{
    cFALSE,
    cTRUE
}boolean_csv;
/*************************************************************/
typedef struct _error
{
    char* error_type;
    char* error_msg;
}error;
static error _index_invild_error = {"index error","The index may "};
static error _file_read_error = {"file read error","catch a error when reading the file"};
static error _cell_parser_error = {"cell parse error","please check your data,have a invild token:{\'\\r\',\'\\n\'"};
static error _token_parser_error = {"token parse error","something wrong in the \',\'"};
static error _filestream_error = {"file colse error","the file stream colse unsuccessed"};
static error _cell_type_error = {"Cell type invild","please check this cell's type"};
#define RAISE(ERROR) do{printf("ERROR:\n[%s]:%s\n",ERROR.error_type,ERROR.error_msg);system("pause");system("exit");}while(0)
/*************************************************************/
static STDCAL(column*) columns_parse(const char* first_line);//传入第一行 解析出列名和索引数 返回一个col序列
static STDCAL(void) columns_free(column* columns);//释放col序列的内存
static STDCAL(column*) get_column(column* col_obj,size_t index);//获取col序列中的第index个col对象
static STDCAL(size_t) columns_width(column* col_obj);//获取col对象的宽度 返回值会比实际元素个数少1 因为从0开始计数,但是df对象中的width是实际元素个数
static STDCAL(cell) cell_parser(const char* one_cell);//从字符串中解析出cell
static STDCAL(void) new_series(column* col_obj);
static STDCAL(void) putcell(column* col_obj,cell* cell_obj);
/************************************************************************************/
#define INIT_SERIES_SIZE (256)
static STDCAL(void) new_series(column* col_obj)//初始化series
{
    col_obj->series.size = INIT_SERIES_SIZE;
    col_obj->series.top = 0;
    col_obj->series.cell_table = (cell*)malloc(INIT_SERIES_SIZE*sizeof(cell));
}
static STDCAL(void) putcell(column* col_obj,cell* cell_obj)//将一个cell放入columns中
{
    assert(col_obj->series.size!=0);
    if(col_obj->series.size <= 1 + col_obj->series.top)/////////////////////////////////////////////////////////////////////
    {
        while(col_obj->series.size <= 1 + col_obj->series.top)col_obj->series.size += col_obj->series.size>>1;///////////扩容
        col_obj->series.cell_table = (cell*)realloc(col_obj->series.cell_table,(col_obj->series.size)*sizeof(cell));///////////////////////////////
    }
    cell* ret = col_obj->series.cell_table + col_obj->series.top;
    *ret = *cell_obj;
    col_obj->series.top += 1;      
}
/************************************************************************************/
/** columns_op **/
static STDCAL(column*) columns_parse(const char* first_line)
{
    const char* ch =first_line;
    column* columns = (column*)malloc(sizeof(column));
    columns->coltype = rnull;
    new_series(columns);
    memset(columns->name,'\0',100);
    columns->next_col = NULL;
    column* header_col = columns;
    char buf[COL_NAME_LENGTH_MAX] = {'\0'};
    size_t index = 0;
    size_t iter = 0;
    while (*ch != '\r'&&*ch != '\n')
    {
        switch (*ch)
        {
            case ',':
                {
                strcpy(columns->name,buf);//加载缓冲
                columns->index = index++;
                columns->next_col = (column*)malloc(sizeof(column));
                new_series(columns->next_col);
                columns = columns->next_col;//进入下一个col
                columns->coltype = rnull;
                memset(columns->name,'\0',100);//初始化col
                columns->next_col = NULL;
                _strset(buf,'\0');//清空缓冲
                iter = 0;
                break;
                }
            case '\"':
                break;//跳过\"
            default:
                buf[iter++] = *ch;
                break;
        }
        ch++;
    }
    if(*ch=='\r' || *ch=='\n')//加载最后一个col
    {
        strcpy(columns->name,buf);
        columns->index = index++;
        columns->next_col = NULL;
    }
    return header_col;
}
static STDCAL(void) columns_free(column* columns)
{
    if (columns!=NULL)
    {
        columns_free(columns->next_col);
        free(columns->series.cell_table);
        free(columns);
    }
}
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
static STDCAL(size_t) columns_width(column* col_obj)
{
    assert(col_obj!=NULL);
    while (col_obj->next_col!=NULL)col_obj = col_obj->next_col;
    return col_obj->index;
}

/************************************************************************************/
/** cell_op **/
#define CELLBUF_STR_LENGTH 200
#define LINEBUF_STR_LENGTH 2048
static STDCAL(boolean_csv) cell_parser_interge(const char* one_cell);
static STDCAL(boolean_csv) cell_parser_float(const char* one_cell);
static STDCAL(boolean_csv) cell_parser_nan(const char* one_cell);
static STDCAL(boolean_csv) cell_parser_interge(const char* one_cell)//判断是否为整数
{
    if(*one_cell=='-')one_cell++;
    while (*one_cell!='\0')
    {
        if(!(*one_cell<='9'&&*one_cell>='0'))return cFALSE;
        one_cell++;
    }
    return cTRUE;
}
static STDCAL(boolean_csv) cell_parser_float(const char* one_cell)//判断是否为浮点数
{
    int count = 0;
    if(*one_cell=='-')one_cell++;
    while (*one_cell!='\0'&&count<=1)
    {
        if(*one_cell=='.')count++;
        else if(!(*one_cell<='9'&&*one_cell>='0'))return cFALSE;
        one_cell++;
    }
    if(count!=1)return cFALSE;
    return cTRUE;
}
static STDCAL(boolean_csv) cell_parser_nan(const char* one_cell)
{
    if(*one_cell=='\0')return cTRUE;
    else return cFALSE;
}
static STDCAL(cell) cell_parser(const char* one_cell)
{
    cell ret;
    if(cell_parser_interge(one_cell))
    {
        ret.type=rint;
        ret.data.integer_num = atoi(one_cell);
    }
    else if(cell_parser_float(one_cell))
    {
        ret.type = rfloat;
        ret.data.float_num = atof(one_cell);
    }
    else if(cell_parser_nan(one_cell))
    {
        ret.type = rnan;
        strcpy(ret.data.nan,"NAN\0");
    }
    else
    {
        ret.type = rchar;
        memset(ret.data.char_ch,'\0',200);
        if(*one_cell == '\"')//去除" "
        {
            const char* strptr = one_cell+1;
            char buf[CELLBUF_STR_LENGTH] = {'\0'};
            size_t iter =0;
            for(iter = 0; *strptr != '\0'; iter++)buf[iter] = *strptr++;
            buf[iter-1] = '\0';
            strcpy(ret.data.char_ch,buf);
        }
        else strcpy(ret.data.char_ch,one_cell);
    }
    return ret;
}
/************************************************************************************/

static STDCAL(FILE*) reader_getline(FILE* fp,char* buf);//从文件中读取一行
static STDCAL(FILE*) reader_getline(FILE* fp,char* buf)
{
    memset(buf,'\0',2048);
    char ch;//每一行以\r\n\0结尾
    char* bufptr = buf;
    while ((!ferror(fp))&&(!feof(fp)))
    {
        ch = fgetc(fp);
        switch (ch)
        {
            case '\n':
            case '\r':
                *bufptr = ch;
                bufptr++;
                *bufptr = '\0';
                goto reader_getline_out;
                break;
            default:
                *bufptr = ch;
                bufptr++;
                break;
        }
    }
    *bufptr = '\0';
    if(ferror(fp))RAISE(_file_read_error);
    else if(feof(fp))return NULL;//文件读取完毕
    reader_getline_out:
    return fp;
}
#define WHITESPACE_SKIP(strptr) while(*strptr==' ') strptr++
static STDCAL(char*) cellstr_get(char buf[CELLBUF_STR_LENGTH],char* str)
{
    char* strptr = str;
    size_t iter = 0;
    WHITESPACE_SKIP(strptr);
    switch(*strptr)
    {
        case '\"':
        {
            buf[iter++] = '\"';
            strptr++;
            while(*strptr!='\"') 
            {
                switch(*strptr)
                {
                    case '\\': //处理转义字符
                    {
                        strptr++;
                        switch(*strptr)
                        {
                            case '\"':
                                buf[iter++] = '\"';
                                break;
                            case 't':
                                buf[iter++] = '\t';
                                break;
                            case 'a':
                                buf[iter++] = '\a';
                                break;
                            case '\'':
                                buf[iter++] = '\'';
                                break;
                            default:
                                buf[iter++] = '\\';
                                buf[iter++] = *strptr;
                                break;

                        }
                        strptr++;
                        break;
                    }
                    case '\r':
                    case '\n':
                        RAISE(_cell_parser_error);
                        break;
                    default:
                        buf[iter++] = *strptr++;
                        break;
                }
            }
            buf[iter++] = *strptr++;
            break;
        }
        default:
        {
            while(*strptr!=','&& *strptr!='\r'&& *strptr!='\n'&& *strptr != '\0') buf[iter++] = *strptr++;
        }
    }
    return strptr;
}
extern STDCAL(dataframe) read_csv(const char* path)
{
    dataframe ret;
    size_t length;
    size_t width;
    FILE* csvfp = fopen(path,"r");
    char buf[LINEBUF_STR_LENGTH];
    csvfp = reader_getline(csvfp,buf);
    column* col_obj = columns_parse(buf);
    ret.col = col_obj;
    while (!feof(csvfp) && !ferror(csvfp) && csvfp != NULL)
    {
        csvfp = reader_getline(csvfp,buf);
        char* bufptr = buf;
        char cell_buf[CELLBUF_STR_LENGTH] = {'\0'};
        size_t col_index = 0;
        while (!(*bufptr=='\r' || *bufptr=='\n') && *bufptr!='\0')
        {   
            bufptr = cellstr_get(cell_buf,bufptr);
            cell cell_obj = cell_parser(cell_buf);
            putcell(get_column(col_obj,col_index),&cell_obj);
            memset(cell_buf,'\0',CELLBUF_STR_LENGTH);
            switch(*bufptr)
            {
                case ',':
                    bufptr++;
                    col_index++;
                    break;
                case '\r':
                case '\n':
                case '\0':
                    break;
                default:
                    RAISE(_token_parser_error);
                    break;
            }
        }
    }
    fclose(csvfp);
    ret.width = columns_width(col_obj) -1;//-1是因为从0计数
    ret.length = col_obj->series.top -1;//-1是减去最后一次putcell的多余的加法
    ret.calculable = cFALSE;
    return ret;
}
extern STDCAL(void) df_to_csv(dataframe* df,const char* path)
{
    FILE* dstn_fp = fopen(path,"w+");
    size_t wide,len;
    column* col_list[200] = {NULL};
    column* col_obj = df->col;//拷贝一份col,注:该指针会发生改变,若需链表头，可直接访问df->col
    size_t iter;
    for(iter = 0;col_obj!=NULL;iter++)
    {
        col_list[iter] = col_obj;
        col_obj = col_obj->next_col;//此处为col_obj发生改变的地方
    }
    for(len = 0; len < df->length; len++)
    {
        for(wide =0; wide< iter; wide++)
        {
            cell* cell_obj = col_list[wide]->series.cell_table + len;
            switch(cell_obj->type)
            {
                case rint:
                    fprintf_s(dstn_fp,"%lld",cell_obj->data.integer_num);
                break;
                case rfloat:
                    fprintf_s(dstn_fp,"%lf",cell_obj->data.float_num);
                break;
                case rchar:
                    fprintf_s(dstn_fp,"\"%s\"",cell_obj->data.char_ch);
                break;
                case rnull:
                case rnan:
                    fprintf_s(dstn_fp,"");
                break;
            }
            if(wide != iter -1)fprintf_s(dstn_fp,",");
        }
        fprintf_s(dstn_fp,"\r\n");
    }
    if(fclose(dstn_fp)==EOF)RAISE(_filestream_error);
    else printf("\n[file stream colse safely]");
}
/************************************************************************************/
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
#define COLLIST_LEN 200
#define INIT_COLLIST(col_list)
#define CUTOFF_LINE "------------------------------------------------------------------------------"
#define OMIT_LINE ".............................................................................."
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
/************************************************************************************/
